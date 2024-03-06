import std;
import synodic.honesty.test;
import synodic.honesty.terminal;

using namespace synodic::honesty;
using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite colorSuite(
		"color",
		[]() -> generator<TestBase>
		{
			co_yield "construction"_test = []()
			{
				expect_equals(
					terminal::color24_t(58, 90, 64),
					terminal::color24_t(0x3A'5A'40));  // #3A5A40 Hunter Green
			};
		});

	Suite styleSuite(
		"style",
		[]() -> generator<TestBase>
		{
			co_yield "construction"_test = []()
			{
				terminal::color24_t hunterGreen(58, 90, 64);
				terminal::text_style style(hunterGreen);

				auto foreground = style.Foreground();
				assert(foreground.has_value());
				expect(not style.Background().has_value());
				expect(not style.Attribute().has_value());

				auto value = std::get<terminal::color24_t>(foreground.value());
				expect_equals(value, hunterGreen);
			};
		});

	Suite terminalSuite(
		"terminal",
		[]() -> generator<TestBase>
		{
			co_yield "format"_test = []()
			{

			};

			co_yield "format_to"_test = []()
			{
				std::string output;
				terminal::text_style style(terminal::color24_t(58, 90, 64));
				terminal::format_to(std::back_inserter(output), style, "{} = (58,90,64)", "Hunter Green");

				std::string expected;
				std::format_to(
					std::back_inserter(expected),
					"\x1b[38;2;058;090;064m{}\x1b[0m",
					"Hunter Green = (58,90,64)");

				expect_equals(output, expected);
			};

			co_yield "print"_test = []()
			{

			};
		});

	bool registered = Register(styleSuite, colorSuite, terminalSuite);
}
