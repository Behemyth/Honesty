import std;
import synodic.honesty.test;
import synodic.honesty.benchmark;

using namespace synodic::honesty::test::literals;

namespace
{
	synodic::honesty::test::Suite SUITE(
		"timer",
		[]() -> synodic::honesty::test::Generator
		{
			co_yield "construction"_test = [](const synodic::honesty::test::Requirements& requirements)
			{
				synodic::honesty::benchmark::Duration duration;
				{
					synodic::honesty::benchmark::Timer timer(duration);
				}

				requirements.ExpectGreater(duration.count(), 0);
			};
		});

	synodic::honesty::test::SuiteRegistrar _(SUITE);
}
