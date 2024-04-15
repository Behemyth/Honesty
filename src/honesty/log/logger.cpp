module synodic.honesty.log;

namespace synodic::honesty::log
{
	void Logger::LogV(std::string_view fmt, std::format_args args) const
	{
		for (Sink* sink: sinks_)
		{
			sink->LogV(fmt, args);
		}
	}

	void Logger::LogV(const text_style& style, std::string_view fmt, std::format_args args) const
	{
		for (Sink* sink: sinks_)
		{
			sink->LogV(style, fmt, args);
		}
	}
}
