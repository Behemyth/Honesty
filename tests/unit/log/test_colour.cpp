import std;
import synodic.honesty.test;
import synodic.honesty.log;

using namespace synodic::honesty;
using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	constexpr std::uint32_t green = 0x3A'5A'40; // #3A5A40 Hunter Green

	Suite colorSuite(
		"color",
		[]()
		{
			"construction"_test = []()
			{
				ExpectEquals(
					log::color24_t(58, 90, 64),
					log::color24_t(green));
			};
		});

	Suite styleSuite(
		"style",
		[]()
		{
			"construction"_test = []()
			{
				constexpr log::color24_t hunterGreen(58, 90, 64);
				constexpr log::TextStyle style(hunterGreen);

				constexpr auto foreground = style.Foreground();
				Assert(foreground.has_value());
				Expect(not style.Background().has_value());
				Expect(not style.AttributeMask());

				constexpr auto value = std::get<log::color24_t>(foreground.value());
				ExpectEquals(value, hunterGreen);
			};
		});

	Suite terminalSuite(
		"terminal",
		[]()
		{
			"format_to"_test = []()
			{
				std::string output;
				constexpr log::TextStyle style(log::color24_t(58, 90, 64));
				log::format_to(std::back_inserter(output), style, "{} = (58,90,64)", "Hunter Green");

				std::string expected;
				std::format_to(
					std::back_inserter(expected),
					"\x1b[38;2;058;090;064m{}\x1b[0m",
					"Hunter Green = (58,90,64)");

				ExpectEquals(output, expected);
			};

			"format"_test = []()
			{
				std::string output = log::format(
					log::TextStyle(log::color24_t(58, 90, 64)),
					"Hunter Green = (58,90,64)");

				std::string expected = std::format("\x1b[38;2;058;090;064m{}\x1b[0m", "Hunter Green = (58,90,64)");

				ExpectEquals(output, expected);
			};

			"print"_test = []()
			{
				// TODO: How do we capture the output?
			};
		});

	bool registered = RegisterSuite(styleSuite, colorSuite, terminalSuite);
}
