import synodic.honesty.test;

import std;

// TODO: Report compile-time tests

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	Suite SUITE(
		"expectation",
		[](Fixture& fixture) -> Generator
		{
			const std::stringstream& stream = fixture.AttachListener(honesty::log::LevelType::TRACE);

			co_yield FAIL / "error_description"_test = [&]() -> Generator
			{
				// Tests callable for error message        
				const auto description = []() -> std::string
				{
					return "This outputs only on error";
				};

				co_yield "stream_operator"_test = [&](const Requirements& requirements)
				{
					requirements.Expect(
						false,
						"This test will always fail, triggering the message.");
				} << "message on failure";

				// Assert
				{
					// TODO: Enable when the "Fail" tag works
					//requirements.Assert(false, "This outputs only on error");

					//requirements.Assert(true, "This outputs only on error");

					// TODO: Enable when the "Fail" tag works
					//requirements.Assert(false, description);
				}

				co_yield FAIL / "inner"_test = [&](const Requirements&)
				{
				};

				// Expect
				{
					// TODO: Enable when the "Fail" tag works
					//requirements.Expect(false, "This outputs only on error");

					//requirements.Expect(true, "This outputs only on error");

					// TODO: Enable when the "Fail" tag works
					//requirements.Expect(false, description);
				}
			};
		});

	SuiteRegistrar _(SUITE);
}
