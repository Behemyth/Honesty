export module synodic.honesty.log:types;

import std;

namespace synodic::honesty::log
{
	export enum class LevelType
	{
		DEFER,	// Defer logging to the parent level
		TRACE,
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		CRITICAL
	};

	export template<typename T>
	concept mutex = requires(T type) {
		{
			type.lock()
		} -> std::same_as<void>;
		{
			type.unlock()
		} -> std::same_as<void>;
	};
};

