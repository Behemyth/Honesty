export module synodic.honesty.log:types;

namespace synodic::honesty::log
{
	enum class LevelType
	{
		DEFER,	// Defer logging to the parent level
		TRACE,
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		CRITICAL
	};
}
