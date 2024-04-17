module synodic.honesty.log;

namespace synodic::honesty::log
{
	Sink::Sink(Level level) :
		level_(level)
	{
	}

	void Sink::Flush()
	{
	}

	Level Sink::GetLevel() const
	{
		return level_;
	}
}
