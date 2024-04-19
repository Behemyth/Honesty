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
				expect_equals(
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
				log::color24_t hunterGreen(58, 90, 64);
				log::text_style style(hunterGreen);

				auto foreground = style.Foreground();
				assert(foreground.has_value());
				expect(not style.Background().has_value());
				expect(not style.AttributeMask());

				auto value = std::get<log::color24_t>(foreground.value());
				expect_equals(value, hunterGreen);
			};
		});

	Suite terminalSuite(
		"terminal",
		[]() 
		{
			 "format_to"_test = []()
			{
				std::string output;
				log::text_style style(log::color24_t(58, 90, 64));
				log::format_to(std::back_inserter(output), style, "{} = (58,90,64)", "Hunter Green");

				std::string expected;
				std::format_to(
					std::back_inserter(expected),
					"\x1b[38;2;058;090;064m{}\x1b[0m",
					"Hunter Green = (58,90,64)");

				expect_equals(output, expected);
			};

			 "format"_test = []()
			{
				std::string output = log::format(
					log::text_style(log::color24_t(58, 90, 64)),
					"Hunter Green = (58,90,64)");

				std::string expected = std::format("\x1b[38;2;058;090;064m{}\x1b[0m", "Hunter Green = (58,90,64)");

				expect_equals(output, expected);
			};

			 "print"_test = []()
			{
				// TODO: How do we capture the output?
			};
		});

	bool registered = Register(styleSuite, colorSuite, terminalSuite);
}
