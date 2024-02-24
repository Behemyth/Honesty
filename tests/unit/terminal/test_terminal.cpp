import std;
import synodic.honesty.test;
import synodic.honesty.terminal;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto colorGenerator = []() -> TestGenerator
	{
		co_yield "construction"_test = []()
		{
			terminal::color24_t(255, 20, 30);
		};
	};

	Suite colorSuite("Color", colorGenerator);
	bool registeredColorSuite = colorSuite.Register();

	auto terminalGenerator = []() -> TestGenerator
	{
		co_yield "format"_test = []()
		{

		};

		co_yield "format_to"_test = []()
		{
			std::string output;

			terminal::text_style style(terminal::color24_t(255, 20, 30));

			terminal::format_to(
				std::back_inserter(output),
				style,
				"rgb(255,20,30){}{}{}",
				1,
				2,
				3);
			expect_equals(output, "\x1b[38;2;255;020;030mrgb(255,20,30)123\x1b[0m");
		};

		co_yield "print"_test = []()
		{

		};
	};

	Suite terminalSuite("Terminal", terminalGenerator);
	bool registeredTerminalSuite = terminalSuite.Register();
}
