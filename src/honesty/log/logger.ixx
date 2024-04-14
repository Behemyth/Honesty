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
		consteval Logger(std::string_view name);
		consteval Logger(std::string_view name, std::span<Sink*> sinks);

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
		Level level_;

		std::string_view name_;
		std::span<Sink*> sinks_;
	};

	consteval Logger::Logger(std::string_view name) :
		level_(Level::INFO),
		name_(name)
	{
	}

	consteval Logger::Logger(std::string_view name, std::span<Sink*> sinks) :
		level_(Level::INFO),
		name_(name),
		sinks_(sinks)
	{
	}
}
