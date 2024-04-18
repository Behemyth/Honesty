module synodic.honesty.log;

namespace
{
	std::map<std::size_t, synodic::honesty::log::Logger> LOGGERS;
	const synodic::honesty::log::Logger& ROOT_LOGGER =
		LOGGERS.try_emplace(std::hash<std::string_view>{}(""), "").first->second;
}

namespace synodic::honesty::log
{
	Logger::Logger(std::string_view name) :
		level_(LevelType::DEFER),
		parent_(nullptr),
		name_(name),
		propagate_(true),
		disabled_(false)
	{
	}

	void Logger::LogV(LevelType level, std::string_view fmt, std::format_args args) const
	{
		for (Sink* sink: sinks_)
		{
			sink->LogV(level, fmt, args);
		}
	}

	std::string_view Logger::Name() const
	{
		return name_;
	}

	LevelType Logger::Level() const
	{
		return level_;
	}

	LevelType Logger::EffectiveLevel() const
	{
		if (level_ != LevelType::DEFER)
		{
			return level_;
		}
		if (parent_)
		{
			return parent_->EffectiveLevel();
		}
		return LevelType::TRACE;
	}

	void Logger::SetLevel(LevelType level)
	{
		level_ = level;
	}

	void Logger::AddSink(Sink* sink) const
	{
		sinks_.push_back(sink);
	}

	void Logger::RemoveSink(Sink* sink) const
	{
		if (const auto iterator = std::ranges::find(sinks_, sink); iterator != sinks_.end())
		{
			sinks_.erase(iterator);
		}
	}

	std::span<Sink*> Logger::Sinks() const
	{
		return sinks_;
	}

	bool Logger::HasSink() const
	{
		bool parentHasSink = false;
		if (parent_)
		{
			parentHasSink = parent_->HasSink();
		}
		return !sinks_.empty() || parentHasSink;
	}

	Logger* Logger::Parent() const
	{
		return parent_;
	}

	std::span<Logger*> Logger::GetChildren() const
	{
		return children_;
	}

	bool Logger::Propagate() const
	{
		return propagate_;
	}

	bool Logger::Disabled() const
	{
		return disabled_;
	}

	void Logger::SetPropagate(bool propagate)
	{
		propagate_ = propagate;
	}

	void Logger::SetDisabled(bool disabled)
	{
		disabled_ = disabled;
	}

	Logger::Logger() :
		level_(LevelType::TRACE),
		parent_(nullptr),
		name_(""),
		propagate_(false),
		disabled_(false)
	{
	}

	const Logger& GetRootLogger()
	{
		return ROOT_LOGGER;
	}

	Logger& GetLogger(std::string_view name)
	{
		return LOGGERS.try_emplace(std::hash<std::string_view>{}(name), name).first->second;
	}
}
