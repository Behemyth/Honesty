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
			co_yield "description"_test = [](const Requirements& requirements)
			{
				//	requirements.Expect(false) << "This outputs only on error";
				//	requirements.Expect(false).ErrorMessage = "";

				//	requirements.Expect(true) << "This outputs only on error";
				//	requirements.Expect(true).ErrorMessage = "";

				//	// Tests callable for error message
				//	requirements.Expect(false) << []()
				//	{
				//	};
			};
		});

	SuiteRegistrar _(expectationSuite);
}
