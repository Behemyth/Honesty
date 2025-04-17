import std;
import synodic.honesty.test;
import synodic.honesty.metric;

using namespace synodic::honesty::test::literals;

namespace
{
	synodic::honesty::test::Suite SUITE(
		"context",
		[]() -> synodic::honesty::test::Generator
		{
			co_yield "construction"_test = [](const synodic::honesty::test::Requirements& requirements)
			{
				synodic::honesty::metric::RegressionContext regressionContext;
				synodic::honesty::metric::TimedContext timedContext(std::chrono::nanoseconds(1));
			};

			// Now that we mark that they work, let's use them
			synodic::honesty::metric::RegressionContext regressionContext;
			synodic::honesty::metric::TimedContext timedContext(std::chrono::nanoseconds(1));

			/* co_yield "empty"_test = [](const synodic::honesty::test::Requirements& requirements, const auto&
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

	synodic::honesty::test::SuiteRegistrar _(SUITE);
}
