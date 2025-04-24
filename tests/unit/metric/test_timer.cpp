import std;
import synodic.honesty.test;
import synodic.honesty.metric;

using namespace honesty::test::literals;

namespace
{
	honesty::test::Suite SUITE(
		"timer",
		[]() -> honesty::test::Generator
		{
			co_yield "construction"_test = [](const honesty::test::Requirements& requirements)
			{
				honesty::metric::Duration duration;
				{
					honesty::metric::Timer timer(duration);
				}

				// The system time is accurate only to a certain degree, so we can't guarantee that the duration is
				//	greater than 0
				requirements.ExpectGreaterEqual(duration.count(), 0);
			};
		});

	honesty::test::SuiteRegistrar _(SUITE);
}
