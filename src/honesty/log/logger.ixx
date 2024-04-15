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
		constexpr Logger(std::string_view name);
		constexpr Logger(std::string_view name, std::span<Sink*> sinks);

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
		std::span<Sink*> sinks_;
	};

	constexpr Logger::Logger(std::string_view name) :
		level_(Level::INFO),
		name_(name)
	{
	}

	constexpr Logger::Logger(std::string_view name, std::span<Sink*> sinks) :
		level_(Level::INFO),
		name_(name),
		sinks_(sinks)
	{
	}
}
