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
		inline void Log(Level level, std::format_string<Args...> fmt, Args&&... args) const
		{
			LogV(level, fmt.get(), std::make_format_args(args...));
		}

		// Type-erased logging similar to std::format
		void LogV(Level level, std::string_view fmt, std::format_args args) const;

		template<class... Args>
		inline void Trace(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(Level::TRACE, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Debug(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(Level::DEBUG, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Info(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(Level::INFO, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Warning(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(Level::WARNING, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Error(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(Level::ERROR, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Critical(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(Level::CRITICAL, fmt, std::forward<Args>(args)...);
		}

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
