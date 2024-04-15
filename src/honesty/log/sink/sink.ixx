export module synodic.honesty.log:sink;

import :colour;
import std;

namespace synodic::honesty::log
{
	export class Sink
	{
	public:
		Sink()			= default;
		virtual ~Sink() = default;

		template<class... Args>
		inline void Log(std::format_string<Args...> fmt, Args&&... args)
		{
			LogV(fmt.get(), std::make_format_args(args...));
		}

		template<class... Args>
		inline void Log(const text_style& style, std::format_string<Args...> fmt, Args&&... args)
		{
			LogV(style, fmt.get(), std::make_format_args(args...));
		}

		virtual void LogV(std::string_view fmt, std::format_args args)							= 0;
		virtual void LogV(const text_style& style, std::string_view fmt, std::format_args args) = 0;

	private:
	};
}
