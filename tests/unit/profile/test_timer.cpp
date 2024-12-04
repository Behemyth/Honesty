import std;
import synodic.honesty.test;
import synodic.honesty.profile;

using namespace synodic::honesty::test::literals;

namespace
{
	synodic::honesty::test::Suite SUITE(
		"timer",
		[]() -> synodic::honesty::test::Generator
		{
			co_yield "construction"_test = [](const synodic::honesty::test::Requirements& requirements)
			{
				synodic::honesty::profile::Duration duration;

				requirements.Expect(not duration.has_value());

				{
					synodic::honesty::profile::Timer timer(duration);
				}

				requirements.Expect(duration.has_value());
				requirements.ExpectGreater(duration.value().count(), 0);
			};
		});

	synodic::honesty::test::SuiteRegistrar _(SUITE);
}
