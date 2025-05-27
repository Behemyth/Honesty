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
			co_yield "error_description"_test = [&]() -> Generator
			{
				const std::stringstream& stream = fixture.AttachListener(honesty::log::LevelType::TRACE);

				// Tests callable for error message        
				const auto description = []() -> std::string
				{
					return "ERROR";
				};

				co_yield FAIL / "assert_fails"_test = [&](const Requirements& requirements)
				{
					requirements.Assert(false, "ERROR");
					requirements.Assert(false, description);
				};

				co_yield "assert_succeeds"_test = [&](const Requirements& requirements)
				{
					requirements.Assert(true, "ERROR");
					requirements.Assert(true, description);
				};

				co_yield FAIL / "expect_fails"_test = [&](const Requirements& requirements)
				{
					requirements.Expect(false, "ERROR");
					requirements.Expect(false, description);
				};

				co_yield "expect_succeeds"_test = [&](const Requirements& requirements)
				{
					requirements.Expect(true, "ERROR");
					requirements.Expect(true, description);
				};

				requirements.Expect(
					stream.view().empty(),
					"Nothing should be present in the stream.");
			};
		});

	SuiteRegistrar _(SUITE);
}
