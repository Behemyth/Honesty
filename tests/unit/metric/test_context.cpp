import std;
import synodic.honesty.test;
import synodic.honesty.metric;

using namespace honesty::test::literals;

namespace
{
	honesty::test::Suite SUITE(
		"context",
		[]() -> honesty::test::Generator
		{
			co_yield "construction"_test = [](const honesty::test::Requirements& requirements)
			{
				honesty::metric::RegressionContext regressionContext;
				honesty::metric::TimedContext timedContext(std::chrono::nanoseconds(1));
			};

			// Now that we mark that they work, let's use them
			honesty::metric::RegressionContext regressionContext;
			honesty::metric::TimedContext timedContext(std::chrono::nanoseconds(1));

			/* co_yield "empty"_test = [](const honesty::test::Requirements& requirements, const auto&
			   context)
					{
						context.Measure(
							[]()
							{
							});
					} | parameterization;*/
			regressionContext.Measure(
				[]()
				{
				});

			timedContext.Measure(
				[]()
				{
				});
		});

	honesty::test::SuiteRegistrar _(SUITE);
}
