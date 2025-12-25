/**
 * @file windows.cpp
 * @brief Windows-specific resource collection using process APIs
 *
 * Implements resource collection using:
 * - GetProcessTimes for CPU usage
 * - GetProcessMemoryInfo for memory metrics
 */

module synodic.honesty.metric;

import std;

extern "C"
{
	// Minimal Windows API declarations to avoid header pollution
	// These are stable ABI - safe to declare directly

	using HANDLE = void*;
	using DWORD = unsigned long;
	using BOOL = int;
	using LONGLONG = long long;
	using SIZE_T = unsigned long long;

	// Process memory counters
	struct PROCESS_MEMORY_COUNTERS
	{
		DWORD cb;
		DWORD PageFaultCount;
		SIZE_T PeakWorkingSetSize;
		SIZE_T WorkingSetSize;
		SIZE_T QuotaPeakPagedPoolUsage;
		SIZE_T QuotaPagedPoolUsage;
		SIZE_T QuotaPeakNonPagedPoolUsage;
		SIZE_T QuotaNonPagedPoolUsage;
		SIZE_T PagefileUsage;
		SIZE_T PeakPagefileUsage;
	};

	// FILETIME for CPU time calculation
	struct FILETIME
	{
		DWORD dwLowDateTime;
		DWORD dwHighDateTime;
	};

	// Process functions
	__declspec(dllimport) HANDLE __stdcall GetCurrentProcess();
	__declspec(dllimport) BOOL __stdcall GetProcessMemoryInfo(
		HANDLE Process, PROCESS_MEMORY_COUNTERS* ppsmemCounters, DWORD cb);
	__declspec(dllimport) BOOL __stdcall GetProcessTimes(
		HANDLE hProcess, FILETIME* lpCreationTime, FILETIME* lpExitTime,
		FILETIME* lpKernelTime, FILETIME* lpUserTime);
	__declspec(dllimport) void __stdcall GetSystemTimeAsFileTime(FILETIME* lpSystemTimeAsFileTime);
}

// Link required libraries
#pragma comment(lib, "psapi.lib")

namespace honesty::metric
{
	namespace
	{
		/// Convert FILETIME to 64-bit value (100-nanosecond intervals)
		LONGLONG FileTimeToInt64(const FILETIME& ft)
		{
			return (static_cast<LONGLONG>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
		}
	}

	struct ResourceCollector::Impl
	{
		ResourceConfig config;
		bool initialized = false;

		// Previous times for CPU delta calculation
		LONGLONG prevProcessTime = 0;
		LONGLONG prevSystemTime = 0;

		Impl(ResourceConfig cfg) : config(std::move(cfg)) {}

		~Impl() = default;

		bool InitializeCpu()
		{
			if (!config.collectCpu)
			{
				return true;
			}

			// Use GetProcessTimes for per-process CPU measurement
			// This is more reliable than PDH for measuring our own process

			// Initialize baseline times
			FILETIME creation, exit, kernel, user, systemTime;
			GetProcessTimes(GetCurrentProcess(), &creation, &exit, &kernel, &user);
			GetSystemTimeAsFileTime(&systemTime);

			prevProcessTime = FileTimeToInt64(kernel) + FileTimeToInt64(user);
			prevSystemTime = FileTimeToInt64(systemTime);

			return true;
		}

		double SampleCpu()
		{
			if (!config.collectCpu)
			{
				return 0.0;
			}

			FILETIME creation, exit, kernel, user, systemTime;
			if (!GetProcessTimes(GetCurrentProcess(), &creation, &exit, &kernel, &user))
			{
				return 0.0;
			}
			GetSystemTimeAsFileTime(&systemTime);

			LONGLONG currentProcessTime = FileTimeToInt64(kernel) + FileTimeToInt64(user);
			LONGLONG currentSystemTime = FileTimeToInt64(systemTime);

			LONGLONG processTimeDelta = currentProcessTime - prevProcessTime;
			LONGLONG systemTimeDelta = currentSystemTime - prevSystemTime;

			// Update previous times for next sample
			prevProcessTime = currentProcessTime;
			prevSystemTime = currentSystemTime;

			if (systemTimeDelta <= 0)
			{
				return 0.0;
			}

			// Calculate CPU percentage
			// Note: This can exceed 100% on multi-core systems
			double cpuUsage =
				(static_cast<double>(processTimeDelta) / static_cast<double>(systemTimeDelta)) * 100.0;

			return cpuUsage;
		}
	};

	// Declare GetCurrentProcessId here since we need it in Impl
	extern "C" __declspec(dllimport) DWORD __stdcall GetCurrentProcessId();

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
		if (impl_->config.collectMemory)
		{
			PROCESS_MEMORY_COUNTERS pmc;
			pmc.cb = sizeof(pmc);
			if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
			{
				sample.memoryUsageBytes = pmc.WorkingSetSize;
				sample.peakMemoryBytes = pmc.PeakWorkingSetSize;
			}
		}

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
