/**
 * @file windows.cpp
 * @brief Windows-specific platform tuning for benchmark stability
 *
 * Implements platform tuning using Windows APIs:
 * - timeBeginPeriod/timeEndPeriod for timer resolution
 * - SetPriorityClass for process priority
 * - SetThreadPriority for thread priority
 * - SetThreadAffinityMask for CPU pinning
 */

module synodic.honesty.metric;

import std;
import synodic.honesty.log;

// Windows headers - linked via CMake
#pragma comment(lib, "winmm.lib")

extern "C"
{
	// Minimal Windows API declarations to avoid header pollution
	// These are stable ABI - safe to declare directly

	// Process/Thread handles
	using HANDLE = void*;
	using DWORD = unsigned long;
	using DWORD_PTR = unsigned long long;
	using BOOL = int;
	using UINT = unsigned int;
	using MMRESULT = UINT;

	// Constants
	constexpr DWORD HIGH_PRIORITY_CLASS = 0x00000080;
	constexpr DWORD NORMAL_PRIORITY_CLASS = 0x00000020;
	constexpr int THREAD_PRIORITY_HIGHEST = 2;
	constexpr int THREAD_PRIORITY_NORMAL = 0;
	constexpr DWORD TIMERR_NOERROR = 0;

	// Process functions
	__declspec(dllimport) HANDLE __stdcall GetCurrentProcess();
	__declspec(dllimport) HANDLE __stdcall GetCurrentThread();
	__declspec(dllimport) DWORD __stdcall GetPriorityClass(HANDLE hProcess);
	__declspec(dllimport) BOOL __stdcall SetPriorityClass(HANDLE hProcess, DWORD dwPriorityClass);
	__declspec(dllimport) int __stdcall GetThreadPriority(HANDLE hThread);
	__declspec(dllimport) BOOL __stdcall SetThreadPriority(HANDLE hThread, int nPriority);
	__declspec(dllimport) DWORD_PTR __stdcall SetThreadAffinityMask(HANDLE hThread, DWORD_PTR dwThreadAffinityMask);

	// Timer functions (from winmm.dll)
	__declspec(dllimport) MMRESULT __stdcall timeBeginPeriod(UINT uPeriod);
	__declspec(dllimport) MMRESULT __stdcall timeEndPeriod(UINT uPeriod);

	// System info
	struct SYSTEM_INFO
	{
		union
		{
			DWORD dwOemId;
			struct
			{
				unsigned short wProcessorArchitecture;
				unsigned short wReserved;
			} s;
		};
		DWORD dwPageSize;
		void* lpMinimumApplicationAddress;
		void* lpMaximumApplicationAddress;
		DWORD_PTR dwActiveProcessorMask;
		DWORD dwNumberOfProcessors;
		DWORD dwProcessorType;
		DWORD dwAllocationGranularity;
		unsigned short wProcessorLevel;
		unsigned short wProcessorRevision;
	};

	__declspec(dllimport) void __stdcall GetSystemInfo(SYSTEM_INFO* lpSystemInfo);
}

namespace honesty::metric
{
	namespace
	{
		auto& PlatformLogger()
		{
			static log::Logger logger = []()
			{
				log::Logger l = log::RootLogger().CreateLogger("honesty.metric.platform");
				l.SetLevel(log::LevelType::DEBUG);
				return l;
			}();
			return logger;
		}
	}

	struct PlatformTuner::Impl
	{
		PlatformConfig config;
		bool isActive = false;

		// Saved state for restoration
		DWORD originalPriorityClass = 0;
		int originalThreadPriority = 0;
		DWORD_PTR originalAffinityMask = 0;
		bool timerPeriodSet = false;
		bool priorityClassChanged = false;
		bool threadPriorityChanged = false;
		bool affinityChanged = false;

		explicit Impl(PlatformConfig cfg)
			: config(std::move(cfg))
		{
		}

		PlatformState Setup()
		{
			PlatformState state;

			if (isActive)
			{
				state.warnings.emplace_back("Platform tuning already active");
				return state;
			}

			PlatformLogger().Debug("Setting up Windows platform tuning");

			// Increase timer resolution to 1ms
			if (config.increaseTimerResolution)
			{
				if (timeBeginPeriod(1) == TIMERR_NOERROR)
				{
					timerPeriodSet = true;
					PlatformLogger().Debug("Timer resolution set to 1ms");
				}
				else
				{
					state.warnings.emplace_back("Failed to set timer resolution to 1ms");
				}
			}

			// Elevate process priority
			if (config.elevateProcessPriority)
			{
				HANDLE process = GetCurrentProcess();
				originalPriorityClass = GetPriorityClass(process);

				if (originalPriorityClass != 0)
				{
					if (SetPriorityClass(process, HIGH_PRIORITY_CLASS))
					{
						priorityClassChanged = true;
						PlatformLogger().Debug("Process priority elevated to HIGH_PRIORITY_CLASS");
					}
					else
					{
						state.warnings.emplace_back("Failed to elevate process priority (may require admin rights)");
						state.recommendations.emplace_back("Run as administrator for more stable benchmarks");
					}
				}

				// Elevate thread priority
				HANDLE thread = GetCurrentThread();
				originalThreadPriority = GetThreadPriority(thread);

				if (SetThreadPriority(thread, THREAD_PRIORITY_HIGHEST))
				{
					threadPriorityChanged = true;
					PlatformLogger().Debug("Thread priority elevated to THREAD_PRIORITY_HIGHEST");
				}
				else
				{
					state.warnings.emplace_back("Failed to elevate thread priority");
				}
			}

			// Pin to CPU core 0
			if (config.pinToCpu)
			{
				HANDLE thread = GetCurrentThread();
				
				// Get system info to determine available processors
				SYSTEM_INFO sysInfo;
				GetSystemInfo(&sysInfo);

				// Use first available processor from the mask
				DWORD_PTR targetMask = 1;  // Core 0
				if ((sysInfo.dwActiveProcessorMask & targetMask) == 0)
				{
					// Core 0 not available, find first available core
					targetMask = sysInfo.dwActiveProcessorMask & (~sysInfo.dwActiveProcessorMask + 1);
				}

				originalAffinityMask = SetThreadAffinityMask(thread, targetMask);
				if (originalAffinityMask != 0)
				{
					affinityChanged = true;
					PlatformLogger().Debug("Thread pinned to CPU core (mask: {})", targetMask);
				}
				else
				{
					state.warnings.emplace_back("Failed to set thread affinity");
					state.recommendations.emplace_back("CPU affinity helps reduce context switch variance");
				}
			}

			isActive = true;
			state.tuningApplied = timerPeriodSet || priorityClassChanged || 
			                       threadPriorityChanged || affinityChanged;

			if (!state.tuningApplied)
			{
				state.recommendations.emplace_back(
					"No platform tuning could be applied. "
					"Consider running as administrator for better benchmark stability.");
			}

			PlatformLogger().Info("Windows platform tuning complete (active: {})", state.tuningApplied);
			return state;
		}

		void Teardown()
		{
			if (!isActive)
			{
				return;
			}

			PlatformLogger().Debug("Tearing down Windows platform tuning");

			// Restore affinity
			if (affinityChanged)
			{
				SetThreadAffinityMask(GetCurrentThread(), originalAffinityMask);
				affinityChanged = false;
				PlatformLogger().Debug("Thread affinity restored");
			}

			// Restore thread priority
			if (threadPriorityChanged)
			{
				SetThreadPriority(GetCurrentThread(), originalThreadPriority);
				threadPriorityChanged = false;
				PlatformLogger().Debug("Thread priority restored");
			}

			// Restore process priority
			if (priorityClassChanged)
			{
				SetPriorityClass(GetCurrentProcess(), originalPriorityClass);
				priorityClassChanged = false;
				PlatformLogger().Debug("Process priority restored");
			}

			// Restore timer resolution
			if (timerPeriodSet)
			{
				timeEndPeriod(1);
				timerPeriodSet = false;
				PlatformLogger().Debug("Timer resolution restored");
			}

			isActive = false;
			PlatformLogger().Info("Windows platform tuning teardown complete");
		}
	};

	// PlatformTuner implementation

	PlatformTuner::PlatformTuner(PlatformConfig config)
		: impl_(std::make_unique<Impl>(std::move(config)))
	{
	}

	PlatformTuner::~PlatformTuner()
	{
		if (impl_)
		{
			impl_->Teardown();
		}
	}

	PlatformTuner::PlatformTuner(PlatformTuner&&) noexcept = default;
	PlatformTuner& PlatformTuner::operator=(PlatformTuner&&) noexcept = default;

	PlatformState PlatformTuner::Setup()
	{
		return impl_->Setup();
	}

	void PlatformTuner::Teardown()
	{
		impl_->Teardown();
	}

	bool PlatformTuner::IsActive() const noexcept
	{
		return impl_ && impl_->isActive;
	}

	const PlatformConfig& PlatformTuner::Config() const noexcept
	{
		return impl_->config;
	}

	// ScopedPlatformTuner implementation

	ScopedPlatformTuner::ScopedPlatformTuner(PlatformConfig config)
		: tuner_(std::move(config))
		, state_(tuner_.Setup())
	{
	}

	ScopedPlatformTuner::~ScopedPlatformTuner()
	{
		tuner_.Teardown();
	}

	const PlatformState& ScopedPlatformTuner::State() const noexcept
	{
		return state_;
	}

	PlatformTuner& ScopedPlatformTuner::Tuner() noexcept
	{
		return tuner_;
	}
}
