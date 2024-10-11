import synodic.honesty.test;

import std;

// TODO: Report compile-time tests

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"expectation",
		[](const Fixture& fixture) -> Generator
		{
			co_yield "error_description"_test = [&](const Requirements& requirements)
			{
				//std::stringstream stream;
				//const TempLogger logger = fixture.TempLog(stream);

				// Tests callable for error message
				const auto description = []() -> std::string
				{
					return "This outputs only on error";
				};

				// Assert
				{
					// TODO: Enable when the "Fail" tag works
					//requirements.Assert(false, "This outputs only on error");

					requirements.Assert(true, "This outputs only on error");

					// TODO: Enable when the "Fail" tag works
					//requirements.Assert(false, description);
				}

				// Expect
				{
					// TODO: Enable when the "Fail" tag works
					//requirements.Expect(false, "This outputs only on error");

					requirements.Expect(true, "This outputs only on error");

					// TODO: Enable when the "Fail" tag works
					//requirements.Expect(false, description);
				}
			};
		});

	SuiteRegistrar _(SUITE);
}
