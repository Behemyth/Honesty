module synodic.honesty.log;

namespace
{
	std::map<std::size_t, synodic::honesty::log::DynamicLogger> LOGGERS;
	const auto ROOT_LOGGER_NAME = "root";
	const synodic::honesty::log::DynamicLogger& ROOT_LOGGER =
		LOGGERS.try_emplace(std::hash<std::string_view>{}(ROOT_LOGGER_NAME), ROOT_LOGGER_NAME).first->second;
}

namespace synodic::honesty::log
{
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

	DynamicLogger* Logger::Parent() const
	{
		return parent_;
	}

	std::span<DynamicLogger*> Logger::Children() const
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

	DynamicLogger::DynamicLogger(std::string_view name) :
		Logger(name)
	{
	}

	void DynamicLogger::AddSink(Sink* sink) const
	{
		sinks_.push_back(sink);
	}

	void DynamicLogger::RemoveSink(Sink* sink) const
	{
		if (const auto iterator = std::ranges::find(sinks_, sink); iterator != sinks_.end())
		{
			sinks_.erase(iterator);
		}
	}

	const DynamicLogger& GetRootLogger()
	{
		return ROOT_LOGGER;
	}

	DynamicLogger& GetLogger(std::string_view name)
	{
		return LOGGERS.try_emplace(std::hash<std::string_view>{}(name), name).first->second;
	}
}
