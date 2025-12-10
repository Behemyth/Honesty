import std;
import synodic.honesty.test;
import synodic.honesty.metric;

using namespace honesty::test;
using namespace honesty::test::literals;
using namespace honesty::metric;

namespace
{
	Suite SUITE(
		"results",
		[]() -> Generator
		{
			co_yield "default_construction"_test = [](const Requirements& requirements)
			{
			Results results;

			requirements.ExpectEquals(results.iterations, std::size_t{0});
			requirements.ExpectEquals(results.samples, std::size_t{0});
			requirements.ExpectEquals(results.totalDuration.count(), 0);
			requirements.ExpectEquals(results.iterations_per_second, 0.0);
			};

			co_yield "from_accumulator"_test = [](const Requirements& requirements)
			{
			BenchmarkAccumulator accumulator;

			// Push some sample timing values (in nanoseconds)
			accumulator.Push(100.0);
			accumulator.Push(110.0);
			accumulator.Push(105.0);
			accumulator.Push(95.0);
			accumulator.Push(102.0);

			Results results;
			results.FromAccumulator(accumulator, 5000, std::chrono::nanoseconds(1000000)); // 5000 iterations in 1ms

			requirements.ExpectEquals(results.iterations, std::size_t{5000});
			requirements.ExpectEquals(results.samples, std::size_t{5});
			requirements.ExpectEquals(results.totalDuration.count(), 1000000);

			// Mean should be around 102.4 ns
			requirements.Expect(results.mean.count() > 90.0);
			requirements.Expect(results.mean.count() < 120.0);

			// Min should be 95, max should be 110
			requirements.ExpectEquals(results.min.count(), 95.0);
			requirements.ExpectEquals(results.max.count(), 110.0);

			// Variance and stddev should be positive
			requirements.Expect(results.variance.count() > 0.0);
			requirements.Expect(results.stddev.count() > 0.0);

			// Throughput: 5000 iterations / 0.001 seconds = 5,000,000 ops/s
			requirements.Expect(results.iterations_per_second > 4000000.0);
			requirements.Expect(results.iterations_per_second < 6000000.0);
			};

			co_yield "single_sample"_test = [](const Requirements& requirements)
			{
			BenchmarkAccumulator accumulator;
			accumulator.Push(500.0);

			Results results;
			results.FromAccumulator(accumulator, 1, std::chrono::nanoseconds(500));

			requirements.ExpectEquals(results.iterations, std::size_t{1});
			requirements.ExpectEquals(results.samples, std::size_t{1});
			requirements.ExpectEquals(results.mean.count(), 500.0);
			requirements.ExpectEquals(results.min.count(), 500.0);
			requirements.ExpectEquals(results.max.count(), 500.0);
			};

			co_yield "zero_duration_throughput"_test = [](const Requirements& requirements)
			{
			BenchmarkAccumulator accumulator;
			accumulator.Push(100.0);

			Results results;
			results.FromAccumulator(accumulator, 1, std::chrono::nanoseconds(0));

			// With zero duration, throughput should be 0 (not infinity or NaN)
			requirements.ExpectEquals(results.iterations_per_second, 0.0);
			};
		});

	SuiteRegistrar _(SUITE);
}
