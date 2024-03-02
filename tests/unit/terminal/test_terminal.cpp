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
				terminal::color24_t(255, 20, 30);
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

				terminal::text_style style(terminal::color24_t(255, 20, 30));

				terminal::format_to(std::back_inserter(output), style, "rgb(255,20,30){}{}{}", 1, 2, 3);
				// expect_equals(output, "\x1b[38;2;255;020;030mrgb(255,20,30)123\x1b[0m");
			};

			co_yield "print"_test = []()
			{

			};
		});

	bool registered = Register(colorSuite, terminalSuite);
}
