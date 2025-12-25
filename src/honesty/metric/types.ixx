export module synodic.honesty.metric:types;

import std;

namespace honesty::metric
{
	export using Clock = std::chrono::steady_clock;
	export using Duration = Clock::duration;

	/**
	 * @brief Compile-time metrics configuration.
	 *
	 * When disabled, all metric collection compiles to nothing via `if constexpr`.
	 * This provides true zero-cost metrics when disabled in production builds.
	 *
	 * Configure via build system defines:
	 *   -DHONESTY_METRICS_DISABLED -> All metrics disabled (zero cost)
	 *   (default)                  -> Metrics enabled
	 */
#if defined(HONESTY_METRICS_DISABLED)
	export inline constexpr bool MetricsEnabled = false;
#else
	export inline constexpr bool MetricsEnabled = true;
#endif

	/**
	 * @brief Measures the effective resolution of the clock
	 *
	 * The clock's nominal resolution (e.g., 100ns for steady_clock on Windows)
	 * doesn't reflect actual measurement granularity. This function empirically
	 * measures the smallest observable time delta.
	 */
	export inline Duration MeasureClockResolution() noexcept
	{
		// Take multiple samples and find the minimum non-zero delta
		constexpr int samples = 100;
		Duration minDelta = Duration::max();

		for (int i = 0; i < samples; ++i)
		{
			auto t0 = Clock::now();
			auto t1 = Clock::now();

			// Spin until we see a change
			while (t1 == t0)
			{
				t1 = Clock::now();
			}

			auto delta = t1 - t0;
			if (delta < minDelta)
			{
				minDelta = delta;
			}
		}

		return minDelta;
	}

	/**
	 * @brief Cached clock resolution (computed once on first use)
	 */
	export inline Duration GetClockResolution() noexcept
	{
		static const Duration resolution = MeasureClockResolution();
		return resolution;
	}
}
