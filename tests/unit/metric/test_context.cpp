import std;
import synodic.honesty.test;
import synodic.honesty.metric;

using namespace honesty::test;
using namespace honesty::test::literals;
using namespace honesty::metric;

namespace
{
	Suite SUITE(
		"context",
		[]() -> Generator
		{
			co_yield SILENT / "timed_context_construction"_test = [](const Requirements& requirements)
			{
				TimedContext context(std::chrono::milliseconds(1));
				// Construction should not throw
				requirements.Assert(true);
			};

			co_yield SILENT / "regression_context_construction"_test = [](const Requirements& requirements)
			{
				RegressionContext context;
				// Construction should not throw
				requirements.Assert(true);
			};

			co_yield SILENT / "timed_context_measure"_test = [](const Requirements& requirements)
			{
			TimedContext context(std::chrono::milliseconds(10));

			Results results = context.Measure([]() {
			// Simple work to measure
			int sum = 0;
			for (int i = 0; i < 100; ++i)
			{
			sum += i;
			}
			DoNotOptimize(sum);
			});

			requirements.Expect(results.iterations > std::size_t{0}, "Should have at least one iteration");
			requirements.Expect(results.samples > std::size_t{0}, "Should have at least one sample");
			requirements.Expect(results.totalDuration.count() > 0, "Total duration should be positive");
			requirements.Expect(results.mean.count() > 0.0, "Mean should be positive");
			};

			co_yield SILENT / "regression_context_measure"_test = [](const Requirements& requirements)
			{
			RegressionContext context;

			Results results = context.Measure([]() {
			// Empty lambda to measure minimal overhead
			});

			requirements.Expect(results.iterations > std::size_t{0}, "Should have at least one iteration");
			requirements.Expect(results.samples > std::size_t{0}, "Should have at least one sample");
			};

			co_yield SILENT / "regression_context_get_results"_test = [](const Requirements& requirements)
			{
			RegressionContext context;

			// Measure something
			context.Measure([]() {
			int x = 42;
			DoNotOptimize(x);
			});

			// GetResults should return the same results as the last Measure call
			const Results& storedResults = context.GetResults();

			requirements.Expect(storedResults.iterations > std::size_t{0}, "Stored results should have iterations");
			requirements.Expect(storedResults.samples > std::size_t{0}, "Stored results should have samples");
			};

			co_yield SILENT / "results_statistics"_test = [](const Requirements& requirements)
			{
			TimedContext context(std::chrono::milliseconds(50));

			Results results = context.Measure([]() {
			// Do some measurable work
			int sum = 0;
			for (int i = 0; i < 1000; ++i)
			{
			sum += i;
			}
			DoNotOptimize(sum);
			});

			// Variance and stddev should be non-negative
			requirements.Expect(results.variance.count() >= 0.0, "Variance should be non-negative");
			requirements.Expect(results.stddev.count() >= 0.0, "Stddev should be non-negative");

			// Min should be <= mean <= max (if we have samples)
			if (results.samples > 0)
			{
			requirements.Expect(results.min.count() <= results.mean.count(), "Min should be <= mean");
			requirements.Expect(results.mean.count() <= results.max.count(), "Mean should be <= max");
			}

			// Throughput should be positive
			requirements.Expect(results.iterations_per_second > 0.0, "Throughput should be positive");
			};
		});

	SuiteRegistrar _(SUITE);
}
