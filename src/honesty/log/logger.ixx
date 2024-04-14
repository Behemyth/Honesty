export module synodic.honesty.log:logger;

import std;
import :colour;

namespace synodic::honesty::log
{

	export class Logger
	{
	public:
		consteval Logger(std::string_view name);
		~Logger() = default;

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
		std::string_view name_;
	};

	consteval Logger::Logger(std::string_view name) :
		name_(name)
	{
	}

}
