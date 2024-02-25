module;
#include <cstdio>

export module synodic.honesty.test.logger:console;

import std;
import synodic.honesty.terminal;

export namespace synodic::honesty::logger
{
	class Console
	{
	public:
		template<class... Args>
		void log(std::format_string<Args...> fmt, Args&&... args)
		{
			std::println(fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		void log(const synodic::honesty::terminal::text_style& style, std::format_string<Args...> fmt, Args&&... args)
		{
			synodic::honesty::terminal::println(style, fmt, std::forward<Args>(args)...);
		}

	private:
	};
}
