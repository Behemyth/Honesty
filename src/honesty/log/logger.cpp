module synodic.honesty.log;

namespace synodic::honesty::log
{
	void Logger::LogV(Level level, std::string_view fmt, std::format_args args) const
	{
		for (Sink* sink: sinks_)
		{
			sink->LogV(level, fmt, args);
		}
	}
}
