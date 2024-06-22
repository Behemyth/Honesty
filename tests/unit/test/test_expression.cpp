import synodic.honesty.test;

// TODO: Report compile-time tests

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite expectationSuite(
		"expectation",
		[]() -> Generator
		{
			co_yield "expectation"_test = [](const Requirements& requirements)
			{
				// TODO: except can fail and continue
				// TODO: assert stops when it fails
			};
		});

	SuiteRegistrar _(expectationSuite);
}
