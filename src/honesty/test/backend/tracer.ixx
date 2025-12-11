export module synodic.honesty.test.backend:tracer;

import synodic.honesty.trace;

namespace honesty::test
{
	/**
	 * @brief Tracer categories for the test framework
	 */
	export enum class TracerCategory
	{
		EXECUTE,     // Test execution (suites, tests)
		RUNNER,      // Runner operations
		REPORTER,    // Reporter events
		COUNT
	};

	// Configuration: enabled in debug, disabled in release for zero overhead
#ifdef NDEBUG
	export inline constexpr honesty::trace::TracerConfiguration testTracerConfig{false};
#else
	export inline constexpr honesty::trace::TracerConfiguration testTracerConfig{true};
#endif

	// Global tracer instance for the test framework
	export inline honesty::trace::Tracer<testTracerConfig> TestTracer;
}
