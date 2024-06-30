import synodic.honesty.test;

import std;

// TODO: Report compile-time tests

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"expectation",
		[]() -> Generator
		{
			co_yield "error_description"_test = [](const Requirements& requirements)
			{
				// Tests callable for error message
				const auto description = []() -> std::string
				{
					return "This outputs only on error";
				};

				// Assert
				{
					requirements.Assert(false, "This outputs only on error");

					requirements.Assert(true, "This outputs only on error");

					requirements.Assert(false, description);
				}

				// Expect
				{
					requirements.Expect(false, "This outputs only on error");

					requirements.Expect(true, "This outputs only on error");

					requirements.Expect(false, description);
				}
			};
		});

	SuiteRegistrar _(SUITE);
}
