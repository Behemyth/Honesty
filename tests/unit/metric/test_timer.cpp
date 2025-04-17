import std;
import synodic.honesty.test;
import synodic.honesty.metric;

using namespace synodic::honesty::test::literals;

namespace
{
	synodic::honesty::test::Suite SUITE(
		"timer",
		[]() -> synodic::honesty::test::Generator
		{
			co_yield "construction"_test = [](const synodic::honesty::test::Requirements& requirements)
			{
				synodic::honesty::metric::Duration duration;
				{
					synodic::honesty::metric::Timer timer(duration);
				}

				// The system time is accurate only to a certain degree, so we can't guarantee that the duration is
				//	greater than 0
				requirements.ExpectGreaterEqual(duration.count(), 0);
			};
		});

	synodic::honesty::test::SuiteRegistrar _(SUITE);
}
