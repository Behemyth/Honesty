module synodic.honesty.log;

namespace synodic::honesty::log
{
	Sink::Sink(LevelType level) :
		level_(level)
	{
	}

	void Sink::Flush()
	{
	}

	void Sink::SetFormatter(Formatter* formatter)
	{
	}

	LevelType Sink::Level() const
	{
		return level_;
	}

	void Sink::SetLevel(LevelType level)
	{
		level_ = level;
	}
}
