import std;
import synodic.honesty.test;
import synodic.honesty.benchmark;

using namespace synodic::honesty::test::literals;

namespace
{
	synodic::honesty::test::Suite SUITE(
		"context",
		[]() -> synodic::honesty::test::Generator
		{
			co_yield "regression"_test = [](const synodic::honesty::test::Requirements& requirements)
			{
				synodic::honesty::benchmark::RegressionContext regressionContext;
			};
		});

	synodic::honesty::test::SuiteRegistrar _(SUITE);
}
