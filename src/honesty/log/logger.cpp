module synodic.honesty.log;

namespace synodic::honesty::log
{
	Logger::Logger(std::string_view name) :
		level_(Level::DEFER),
		name_(name)
	{
	}

	void Logger::LogV(Level level, std::string_view fmt, std::format_args args) const
	{
		if (not sinks_.has_value())
		{
			return;
		}

		for (Sink* sink: sinks_.value())
		{
			sink->LogV(level, fmt, args);
		}
	}

	Level Logger::GetLevel() const
	{
		return level_;
	}
}
