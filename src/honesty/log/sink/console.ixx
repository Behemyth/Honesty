export module synodic.honesty.log:sink.console;

import std;
import :colour;
import :sink;

namespace synodic::honesty::log
{
	export class Console : Sink
	{
	public:
		consteval Console() = default;

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
