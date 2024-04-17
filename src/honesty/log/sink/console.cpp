module synodic.honesty.log;

import std;

namespace synodic::honesty::log
{
	void Console::LogV(Level level, std::string_view fmt, std::format_args args)
	{
		std::println("{}", std::vformat(fmt, args));
	}
}
