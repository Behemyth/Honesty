module synodic.honesty.log;

import std;

namespace synodic::honesty::log
{
	void Console::LogV(std::string_view fmt, std::format_args args)
	{
		std::println("{}", std::vformat(fmt, args));
	}

	void Console::LogV(const text_style& style, std::string_view fmt, std::format_args args)
	{
		std::println("{}", log::vformat(style, fmt, args));
	}
}
