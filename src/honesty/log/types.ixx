export module synodic.honesty.log:types;

namespace synodic::honesty::log
{
	enum class Level
	{
		DEFER,	// Defer logging to the parent level. If the logger is the root logger, this will be treated as TRACE.
		TRACE,
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		CRITICAL
	};
}
