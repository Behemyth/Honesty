export module synodic.honesty.test.logger.console;

import std;
import synodic.honesty.terminal;

namespace synodic::honesty::test::logger
{
	export class Console
	{
	public:
		template<class... Args>
		void log(std::format_string<Args...> fmt, Args&&... args)
		{
			std::println(fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		void log(const terminal::text_style& style, std::format_string<Args...> fmt, Args&&... args)
		{
			terminal::println(style, fmt, std::forward<Args>(args)...);
		}

	private:
	};
}