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
	 * @brief A duration type using double representation for statistical calculations.
	 */
	using StatsDuration = std::chrono::duration<double, std::nano>;

	/**
	 * @brief Statistical results from benchmark measurements.
	 *
	 * Populated from BenchmarkAccumulator after measurement completion.
	 * Uses std::chrono types for type-safe duration representation.
	 */
	export struct Results
	{
		// Iteration and sample counts
		std::size_t iterations {};
		std::size_t samples {};

		// Total measurement duration
		Duration totalDuration {};

		// Statistical summary (per iteration)
		StatsDuration mean {};
		StatsDuration variance {};
		StatsDuration stddev {};
		StatsDuration min {};
		StatsDuration max {};

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

			mean	 = StatsDuration(accumulator.template get<synodic::statistics::mean>().result());
			variance = StatsDuration(accumulator.template get<synodic::statistics::variance>().result());
			stddev	 = StatsDuration(accumulator.template get<synodic::statistics::variance>().stddev());

			if (const auto minResult = accumulator.template get<synodic::statistics::min>().result())
			{
				min = StatsDuration(*minResult);
			}

			if (const auto maxResult = accumulator.template get<synodic::statistics::max>().result())
			{
				max = StatsDuration(*maxResult);
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
