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

	// Global tracer instance for the test framework.
	// Uses the non-viral Tracer class which respects TracingEnabled.
	// Configure via -DHONESTY_TRACING_DISABLED or -DHONESTY_TRACING_DEBUG_ONLY
	export inline honesty::trace::Tracer TestTracer;
}
