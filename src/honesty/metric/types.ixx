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
}
