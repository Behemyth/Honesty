module synodic.honesty.log;

import std;

namespace synodic::honesty::log
{
	void Console::LogV(LevelType level, std::string_view fmt, std::format_args args)
	{
		std::println("{}", std::vformat(fmt, args));
	}
}
