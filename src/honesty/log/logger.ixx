export module synodic.honesty.log:logger;

import std;
import :colour;
import :sink;
import :types;

import synodic.honesty.utility;

namespace synodic::honesty::log
{
	export class Logger
	{
	public:
		// Copying is disabled because it would invalidate the parent pointer for any children, and we
		//	don't track children as they are implicit on user lifetimes. Moving does so also, but the r-value cast makes
		//	the motive explicit

		Logger(const Logger& other)							 = delete;
		constexpr Logger(Logger&& other) noexcept			 = default;
		Logger& operator=(const Logger& other)				 = delete;
		constexpr Logger& operator=(Logger&& other) noexcept = default;

		template<class... Args>
		inline void Log(const LevelType level, std::format_string<Args...> fmt, Args&&... args) const
		{
			LogV(level, fmt.get(), std::make_format_args(args...));
		}

		/**
		 * @brief Logs a message with the given level. This is the internal function that all other log functions call
		 * @param level The level of the message
		 * @param fmt The format string
		 * @param args The arguments
		 */
		void LogV(const LevelType level, const std::string_view fmt, const std::format_args args) const
		{
			if (sink_)
			{
				sink_->LogV(level, fmt, args);
			}
		}

		template<class... Args>
		inline void Trace(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(LevelType::TRACE, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Debug(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(LevelType::DEBUG, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Info(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(LevelType::INFO, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Warning(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(LevelType::WARNING, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Error(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(LevelType::ERROR, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Critical(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(LevelType::CRITICAL, fmt, std::forward<Args>(args)...);
		}

		/**
		 * @brief Returns the name of this logger
		 * @return The name
		 */
		std::string_view Name() const
		{
			return name_;
		}

		/**
		 * @brief Returns the level of this logger
		 * @return The level
		 */
		LevelType Level() const
		{
			return level_;
		}

		/**
		 * @brief Returns the level or the first ancestor that isn't DEFER
		 * @return The effective level
		 */
		LevelType EffectiveLevel() const
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

		/**
		 * @brief Sets the level of this logger
		 * @param level The new level
		 */
		void SetLevel(const LevelType level)
		{
			level_ = level;
		}

		/**
		 * @brief Returns the sinks of this logger
		 * @return The sinks
		 */
		Sink* Sink() const
		{
			return sink_;
		}

		void SetSink(log::Sink* sink = nullptr)
		{
			sink_ = sink;
		}

		/**
		 * @brief Checks to see if this logger or its ancestors have any sinks
		 * @return Whether sinks were found
		 */
		bool HasSink() const
		{
			bool parentHasSink = false;
			if (parent_)
			{
				parentHasSink = parent_->HasSink();
			}
			return sink_ || parentHasSink;
		}

		/**
		 * @brief Returns the parent logger
		 * @return The parent logger
		 */
		const Logger* Parent() const
		{
			return parent_;
		}

		/**
		 * @brief Checks if this logger is propagating messages
		 * @return Whether propagation is enabled
		 */
		bool Propagate() const
		{
			return propagate_;
		}

		/**
		 * @brief Checks if this logger is disabled
		 * @return Whether the logger is disabled
		 */
		bool Disabled() const
		{
			return disabled_;
		}

		/**
		 * @brief Sets whether this logger should propagate messages
		 * @param propagate Whether to propagate
		 */
		void SetPropagate(const bool propagate)
		{
			propagate_ = propagate;
		}

		/**
		 * @brief Sets whether this logger is disabled
		 * @param disabled Whether to disable the logger
		 */
		void SetDisabled(const bool disabled)
		{
			disabled_ = disabled;
		}

		Logger CreateLogger(const std::string_view name) const
		{
			return Logger(name, this);
		}

		[[nodiscard]] bool operator==(const Logger& rhs) const
		{
			return name_ == rhs.name_;
		}

		[[nodiscard]] auto operator<=>(const Logger& rhs) const
		{
			return name_ <=> rhs.name_;
		}

	private:
		friend class LoggerRegistry;

		/***
		 * @brief Constructs a logger with the given name. Prevents a user from creating a logger type directly
		 * @param name The name of the logger
		 * @param parent The logger parent to associate with
		 */
		explicit(false) constexpr Logger(const std::string_view name, const Logger* parent) :
			level_(LevelType::DEFER),
			name_(name),
			propagate_(true),
			disabled_(false),
			parent_(parent),
			sink_(nullptr)
		{
		}

		LevelType level_;
		std::string_view name_;

		bool propagate_;
		bool disabled_;

		const Logger* parent_;

		log::Sink* sink_;
	};

	class LoggerRegistry
	{
		static constexpr std::string_view ROOT_LOGGER_NAME = "root";
		static constexpr auto ROOT_LOGGER_HASH			   = utility::Hash(ROOT_LOGGER_NAME);

	public:
		consteval LoggerRegistry() :
			root_(ROOT_LOGGER_NAME, nullptr)
		{
		}

		LoggerRegistry(const LoggerRegistry& other) = delete;
		LoggerRegistry(LoggerRegistry&& other)		= delete;

		LoggerRegistry& operator=(const LoggerRegistry& other) = delete;
		LoggerRegistry& operator=(LoggerRegistry&& other)	   = delete;

		~LoggerRegistry() = default;

		constexpr const Logger& RootLogger() const
		{
			return root_;
		}

	private:
		Logger root_;
	};

}

constinit synodic::honesty::log::LoggerRegistry LOGGER_REGISTRY;

namespace synodic::honesty::log
{

	export constexpr const Logger& RootLogger()
	{
		return LOGGER_REGISTRY.RootLogger();
	}

}
