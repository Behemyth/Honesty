import synodic.honesty.test;

import std;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	Suite SUITE(
		"expectation",
		[](Fixture& fixture) -> Generator
		{
			const std::stringstream& stream = fixture.AttachListener(honesty::log::LevelType::TRACE);

			co_yield "error_description"_test = [&]() -> Generator
			{
				// Tests callable for error message        
				const auto description = []() -> std::string
				{
					return "This outputs only on error";
				};

				co_yield FAIL / "assert"_test = [&](const Requirements& requirements)
				{
					requirements.Assert(false, "This outputs only on error");

					requirements.Assert(true, "This outputs only on error");

					requirements.Assert(false, description);
				};

				co_yield FAIL / "expect"_test = [&](const Requirements& requirements)
				{
					requirements.Expect(false, "This outputs only on error");

					requirements.Expect(true, "This outputs only on error");

					requirements.Expect(false, description);
				};
			};
		});

	SuiteRegistrar _(SUITE);
}
