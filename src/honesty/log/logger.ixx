export module synodic.honesty.log:logger;

import std;
import :colour;
import :sink;
import :types;

namespace synodic::honesty::log
{

	export class Logger
	{
	public:
		Logger(std::string_view name);
		Logger(std::string_view name, Sink& sink);

		~Logger() = default;

		template<class... Args>
		inline void Log(std::format_string<Args...> fmt, Args&&... args) const
		{
			LogV(fmt.get(), std::make_format_args(args...));
		}

		template<class... Args>
		inline void Log(const text_style& style, std::format_string<Args...> fmt, Args&&... args) const
		{
			LogV(style, fmt.get(), std::make_format_args(args...));
		}

		void LogV(std::string_view fmt, std::format_args args) const;
		void LogV(const text_style& style, std::string_view fmt, std::format_args args) const;

	private:
		Level level_;

		std::string_view name_;
		std::vector<Sink*> sinks_;
	};

	Logger::Logger(std::string_view name) :
		level_(Level::INFO),
		name_(name)
	{
	}

	Logger::Logger(std::string_view name, Sink& sink) :
		level_(Level::INFO),
		name_(name)
	{
		sinks_.push_back(&sink);
	}
}
