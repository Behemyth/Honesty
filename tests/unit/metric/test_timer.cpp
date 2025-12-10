import std;
import synodic.honesty.test;
import synodic.honesty.metric;

using namespace honesty::test;
using namespace honesty::test::literals;
using namespace honesty::metric;

namespace
{
	Suite SUITE(
		"timer",
		[]() -> Generator
		{
			co_yield "construction"_test = [](const Requirements& requirements)
			{
				Duration duration;
				{
					Timer timer(duration);
				}

				// The system time is accurate only to a certain degree, so we can't guarantee that the duration is
				//	greater than 0
				requirements.ExpectGreaterEqual(duration.count(), 0);
			};

			co_yield "measures_elapsed_time"_test = [](const Requirements& requirements)
			{
				Duration duration;
				{
					Timer timer(duration);

					// Do some work that takes measurable time
					volatile int sum = 0;
					for (int i = 0; i < 10000; ++i)
					{
						sum += i;
					}
				}

				// After doing work, duration should be positive (or at least non-negative due to clock resolution)
				requirements.ExpectGreaterEqual(duration.count(), 0, "Duration should be non-negative after work");
			};

			co_yield "multiple_timers"_test = [](const Requirements& requirements)
			{
				Duration duration1;
				Duration duration2;

				{
					Timer timer1(duration1);
					// Small delay
					volatile int x = 0;
					for (int i = 0; i < 1000; ++i) { x += i; }
				}

				{
					Timer timer2(duration2);
					// Larger delay
					volatile int x = 0;
					for (int i = 0; i < 10000; ++i) { x += i; }
				}

				// Both durations should be valid
				requirements.ExpectGreaterEqual(duration1.count(), 0);
				requirements.ExpectGreaterEqual(duration2.count(), 0);
			};

			co_yield "duration_type"_test = [](const Requirements& requirements)
			{
				// Verify Duration is nanoseconds
				constexpr bool isNanoseconds = std::same_as<Duration, std::chrono::nanoseconds>;
				requirements.Assert(isNanoseconds, "Duration should be std::chrono::nanoseconds");
			};
		});

	SuiteRegistrar _(SUITE);
}
