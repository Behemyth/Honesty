export module synodic.honesty.metric:results;

import synodic.library;

import :types;
import std;

namespace honesty::metric
{
	/**
	 * @brief Accumulator type for collecting benchmark timing samples.
	 *
	 * Uses synodic-template-library's compile-time composable statistics to
	 * efficiently compute running statistics with Welford's online algorithm.
	 */
	export using BenchmarkAccumulator = synodic::statistics::Accumulator<
		double,
		synodic::statistics::count,
		synodic::statistics::sum,
		synodic::statistics::mean,
		synodic::statistics::variance,
		synodic::statistics::min,
		synodic::statistics::max>;

	/**
	 * @brief Statistical results from benchmark measurements.
	 *
	 * Populated from BenchmarkAccumulator after measurement completion.
	 * All timing values are in nanoseconds for consistency with Duration type.
	 */
	export struct Results
	{
		// Iteration and sample counts
		std::size_t iterations {};
		std::size_t samples {};

		// Total measurement duration
		Duration totalDuration {};

		// Statistical summary (nanoseconds per iteration)
		double mean_ns {};
		double variance_ns {};
		double stddev_ns {};
		double min_ns {};
		double max_ns {};

		// Derived metrics
		double iterations_per_second {};

		/**
		 * @brief Populate results from a BenchmarkAccumulator.
		 *
		 * @param accumulator The accumulator containing collected samples
		 * @param totalIterations Total number of benchmark iterations executed
		 * @param duration Total wall-clock duration of the benchmark
		 */
		void FromAccumulator(
			const BenchmarkAccumulator& accumulator,
			std::size_t totalIterations,
			Duration duration)
		{
			iterations	  = totalIterations;
			samples		  = accumulator.template get<synodic::statistics::count>().result();
			totalDuration = duration;

			mean_ns		= accumulator.template get<synodic::statistics::mean>().result();
			variance_ns = accumulator.template get<synodic::statistics::variance>().result();
			stddev_ns	= accumulator.template get<synodic::statistics::variance>().stddev();

			if (const auto minResult = accumulator.template get<synodic::statistics::min>().result())
			{
				min_ns = *minResult;
			}

			if (const auto maxResult = accumulator.template get<synodic::statistics::max>().result())
			{
				max_ns = *maxResult;
			}

			// Calculate throughput
			if (totalDuration.count() > 0)
			{
				iterations_per_second =
					static_cast<double>(iterations) * 1e9 / static_cast<double>(totalDuration.count());
			}
		}
	};

}
