export module synodic.honesty.log:types;

import std;

namespace honesty::log
{
	export enum class LevelType
	{
		OFF,	  // Compile-time: disable all logging
		DEFER,	  // Defer logging to the parent level
		TRACE,
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		CRITICAL,
		COUNT	  // Sentinel for iteration
	};

	/**
	 * @brief Compile-time logging configuration.
	 *
	 * When MinimumLevel is set higher than a log call's level, the entire
	 * call compiles to nothing via `if constexpr`. This provides true
	 * zero-cost logging when disabled.
	 *
	 * Configure via build system defines:
	 *   -DHONESTY_LOG_LEVEL_OFF      -> All logging disabled
	 *   -DHONESTY_LOG_LEVEL_CRITICAL -> Only CRITICAL
	 *   -DHONESTY_LOG_LEVEL_ERROR    -> ERROR and above
	 *   -DHONESTY_LOG_LEVEL_WARNING  -> WARNING and above
	 *   -DHONESTY_LOG_LEVEL_INFO     -> INFO and above (default)
	 *   -DHONESTY_LOG_LEVEL_DEBUG    -> DEBUG and above
	 *   -DHONESTY_LOG_LEVEL_TRACE    -> All logging enabled
	 */
#if defined(HONESTY_LOG_LEVEL_OFF)
	export inline constexpr LevelType CompileTimeLogLevel = LevelType::OFF;
#elif defined(HONESTY_LOG_LEVEL_CRITICAL)
	export inline constexpr LevelType CompileTimeLogLevel = LevelType::CRITICAL;
#elif defined(HONESTY_LOG_LEVEL_ERROR)
	export inline constexpr LevelType CompileTimeLogLevel = LevelType::ERROR;
#elif defined(HONESTY_LOG_LEVEL_WARNING)
	export inline constexpr LevelType CompileTimeLogLevel = LevelType::WARNING;
#elif defined(HONESTY_LOG_LEVEL_INFO)
	export inline constexpr LevelType CompileTimeLogLevel = LevelType::INFO;
#elif defined(HONESTY_LOG_LEVEL_DEBUG)
	export inline constexpr LevelType CompileTimeLogLevel = LevelType::DEBUG;
#elif defined(HONESTY_LOG_LEVEL_TRACE)
	export inline constexpr LevelType CompileTimeLogLevel = LevelType::TRACE;
#else
	// Default: TRACE (all logging enabled, runtime filtering still applies)
	export inline constexpr LevelType CompileTimeLogLevel = LevelType::TRACE;
#endif

	/**
	 * @brief Check if a log level is enabled at compile time.
	 * @param level The level to check
	 * @return true if the level would pass compile-time filtering
	 */
	export consteval bool IsLevelEnabled(LevelType level)
	{
		if (CompileTimeLogLevel == LevelType::OFF)
			return false;
		return level >= CompileTimeLogLevel;
	}

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

