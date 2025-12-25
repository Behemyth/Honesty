/**
 * @file linux.cpp
 * @brief Linux-specific resource collection using /proc filesystem
 *
 * Implements resource collection using:
 * - /proc/self/stat for CPU times
 * - /proc/self/statm for memory usage
 */

module synodic.honesty.metric;

import std;

namespace honesty::metric
{
	namespace
	{
		/// Get system clock ticks per second
		long GetClockTicksPerSecond()
		{
			static long ticks = sysconf(_SC_CLK_TCK);
			return ticks > 0 ? ticks : 100; // Default to 100 if sysconf fails
		}

		/// Get system page size in bytes
		long GetPageSize()
		{
			static long pageSize = sysconf(_SC_PAGESIZE);
			return pageSize > 0 ? pageSize : 4096; // Default to 4KB
		}
	}

	struct ResourceCollector::Impl
	{
		ResourceConfig config;
		bool initialized = false;

		// Previous times for CPU delta calculation (in clock ticks)
		unsigned long long prevUserTime = 0;
		unsigned long long prevSystemTime = 0;
		std::chrono::steady_clock::time_point prevSampleTime;

		Impl(ResourceConfig cfg) : config(std::move(cfg)) {}

		bool InitializeCpu()
		{
			if (!config.collectCpu)
			{
				return true;
			}

			// Read initial CPU times from /proc/self/stat
			std::ifstream statFile("/proc/self/stat");
			if (!statFile)
			{
				return false;
			}

			std::string line;
			if (!std::getline(statFile, line))
			{
				return false;
			}

			// Parse stat file - format: pid (comm) state ... utime stime ...
			// Fields 14 and 15 are utime and stime (in clock ticks)
			// Need to skip past the comm field which may contain spaces/parens
			auto commStart = line.find('(');
			auto commEnd = line.rfind(')');
			if (commStart == std::string::npos || commEnd == std::string::npos)
			{
				return false;
			}

			std::istringstream iss(line.substr(commEnd + 2)); // Skip ") "
			std::string field;

			// Skip fields 3-13 (state through cnswap predecessor)
			for (int i = 0; i < 11; ++i)
			{
				if (!(iss >> field))
				{
					return false;
				}
			}

			// Fields 14 and 15: utime and stime
			if (!(iss >> prevUserTime >> prevSystemTime))
			{
				return false;
			}

			prevSampleTime = std::chrono::steady_clock::now();
			return true;
		}

		double SampleCpu()
		{
			if (!config.collectCpu)
			{
				return 0.0;
			}

			std::ifstream statFile("/proc/self/stat");
			if (!statFile)
			{
				return 0.0;
			}

			std::string line;
			if (!std::getline(statFile, line))
			{
				return 0.0;
			}

			auto commStart = line.find('(');
			auto commEnd = line.rfind(')');
			if (commStart == std::string::npos || commEnd == std::string::npos)
			{
				return 0.0;
			}

			std::istringstream iss(line.substr(commEnd + 2));
			std::string field;

			for (int i = 0; i < 11; ++i)
			{
				if (!(iss >> field))
				{
					return 0.0;
				}
			}

			unsigned long long currentUserTime, currentSystemTime;
			if (!(iss >> currentUserTime >> currentSystemTime))
			{
				return 0.0;
			}

			auto currentSampleTime = std::chrono::steady_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
								   currentSampleTime - prevSampleTime)
								   .count();

			if (elapsedTime <= 0)
			{
				return 0.0;
			}

			// Calculate CPU time delta in clock ticks
			unsigned long long cpuTimeDelta =
				(currentUserTime - prevUserTime) + (currentSystemTime - prevSystemTime);

			// Update previous values
			prevUserTime = currentUserTime;
			prevSystemTime = currentSystemTime;
			prevSampleTime = currentSampleTime;

			// Convert to percentage
			// cpuTimeDelta is in clock ticks, elapsedTime is in milliseconds
			long ticksPerSecond = GetClockTicksPerSecond();
			double cpuSeconds = static_cast<double>(cpuTimeDelta) / ticksPerSecond;
			double elapsedSeconds = static_cast<double>(elapsedTime) / 1000.0;

			return (cpuSeconds / elapsedSeconds) * 100.0;
		}

		std::pair<std::size_t, std::size_t> SampleMemory()
		{
			if (!config.collectMemory)
			{
				return {0, 0};
			}

			// Read memory from /proc/self/statm
			// Format: size resident shared text lib data dt
			// We want resident (RSS) in pages
			std::ifstream statmFile("/proc/self/statm");
			if (!statmFile)
			{
				return {0, 0};
			}

			unsigned long size, resident;
			if (!(statmFile >> size >> resident))
			{
				return {0, 0};
			}

			std::size_t rssBytes = resident * GetPageSize();

			// For peak memory, read VmHWM from /proc/self/status
			std::size_t peakBytes = 0;
			std::ifstream statusFile("/proc/self/status");
			if (statusFile)
			{
				std::string line;
				while (std::getline(statusFile, line))
				{
					if (line.starts_with("VmHWM:"))
					{
						std::istringstream iss(line.substr(6));
						unsigned long peakKb;
						if (iss >> peakKb)
						{
							peakBytes = peakKb * 1024; // Convert KB to bytes
						}
						break;
					}
				}
			}

			return {rssBytes, peakBytes};
		}
	};
}

// sysconf declaration
extern "C"
{
	long sysconf(int name);
	constexpr int _SC_CLK_TCK = 2;
	constexpr int _SC_PAGESIZE = 30;
}

namespace honesty::metric
{
	ResourceCollector::ResourceCollector(ResourceConfig config)
		: impl_(std::make_unique<Impl>(std::move(config)))
	{
	}

	ResourceCollector::~ResourceCollector() = default;

	ResourceCollector::ResourceCollector(ResourceCollector&&) noexcept = default;
	ResourceCollector& ResourceCollector::operator=(ResourceCollector&&) noexcept = default;

	bool ResourceCollector::Initialize()
	{
		if (impl_->initialized)
		{
			return true;
		}

		if (!impl_->InitializeCpu())
		{
			return false;
		}

		impl_->initialized = true;
		return true;
	}

	ResourceSample ResourceCollector::Sample()
	{
		ResourceSample sample;
		sample.timestamp = Clock::now();

		if (!impl_->initialized)
		{
			return sample;
		}

		// CPU usage
		sample.cpuUsagePercent = impl_->SampleCpu();

		// Memory usage
		auto [rss, peak] = impl_->SampleMemory();
		sample.memoryUsageBytes = rss;
		sample.peakMemoryBytes = peak;

		return sample;
	}

	bool ResourceCollector::IsInitialized() const noexcept
	{
		return impl_->initialized;
	}

	const ResourceConfig& ResourceCollector::Config() const noexcept
	{
		return impl_->config;
	}

	// ScopedResourceSampler implementation

	ScopedResourceSampler::ScopedResourceSampler(ResourceCollector& collector)
		: collector_(collector), startSample_(collector.Sample())
	{
	}

	const ResourceSample& ScopedResourceSampler::StartSample() const noexcept
	{
		return startSample_;
	}

	ResourceSample ScopedResourceSampler::End()
	{
		ResourceSample endSample = collector_.Sample();

		// Compute delta sample
		ResourceSample delta;
		delta.timestamp = endSample.timestamp;
		delta.cpuUsagePercent = endSample.cpuUsagePercent; // CPU is already a rate
		delta.memoryUsageBytes = endSample.memoryUsageBytes - startSample_.memoryUsageBytes;
		delta.peakMemoryBytes = endSample.peakMemoryBytes;

		return delta;
	}
}
