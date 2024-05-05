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
		Logger(const Logger& other)							 = delete;
		constexpr Logger(Logger&& other) noexcept			 = default;
		Logger& operator=(const Logger& other)				 = delete;
		constexpr Logger& operator=(Logger&& other) noexcept = default;

		auto operator<=>(const Logger&) const = default;

		template<class... Args>
		inline void Log(const LevelType level, std::format_string<Args...> fmt, Args&&... args)
		{
			LogV(level, fmt.get(), std::make_format_args(args...));
		}

		/**
		 * @brief Logs a message with the given level. This is the internal function that all other log functions call
		 * @param level The level of the message
		 * @param fmt The format string
		 * @param args The arguments
		 */
		void LogV(const LevelType level, const std::string_view fmt, const std::format_args args)
		{
			for (Sink* sink: Sinks())
			{
				sink->LogV(level, fmt, args);
			}
		}

		template<class... Args>
		inline void Trace(std::format_string<Args...> fmt, Args&&... args)
		{
			Log(LevelType::TRACE, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Debug(std::format_string<Args...> fmt, Args&&... args)
		{
			Log(LevelType::DEBUG, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Info(std::format_string<Args...> fmt, Args&&... args)
		{
			Log(LevelType::INFO, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Warning(std::format_string<Args...> fmt, Args&&... args)
		{
			Log(LevelType::WARNING, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Error(std::format_string<Args...> fmt, Args&&... args)
		{
			Log(LevelType::ERROR, fmt, std::forward<Args>(args)...);
		}

		template<class... Args>
		inline void Critical(std::format_string<Args...> fmt, Args&&... args)
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
		std::span<Sink*> Sinks()
		{
			return sinks_;
		}

		/**
		 * @brief Checks to see if this logger or its ancestors have any sinks
		 * @return Whether sinks were found
		 */
		bool HasSink()
		{
			bool parentHasSink = false;
			if (parent_)
			{
				parentHasSink = parent_->HasSink();
			}
			return !Sinks().empty() || parentHasSink;
		}

		/**
		 * @brief Returns the parent logger
		 * @return The parent logger
		 */
		Logger* Parent() const
		{
			return parent_;
		}

		/**
		 * @brief Returns the direct children of this logger
		 * @return The children
		 */
		std::span<Logger*> Children() const
		{
			return children_;
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

		/**
		 * @brief Adds a sink to this logger
		 * @param sink The sink to add
		 */
		void AddSink(Sink* sink) const
		{
			sinks_.push_back(sink);
		}

		/**
		 * @brief Removes a sink from this logger
		 * @param sink The sink to remove
		 */
		void RemoveSink(Sink* sink) const
		{
			if (const auto iterator = std::ranges::find(sinks_, sink); iterator != sinks_.end())
			{
				sinks_.erase(iterator);
			}
		}

		[[nodiscard]] constexpr bool operator==(const Logger& rhs) const
		{
			return name_ == rhs.name_;
		}

	private:
		friend class LoggerRegistry;

		/***
		 * @brief Constructs a logger with the given name. Prevents a user from creating a logger type directly
		 * @param name The name of the logger
		 */
		explicit(false) Logger(const std::string_view name) :
			level_(LevelType::DEFER),
			parent_(nullptr),
			name_(name),
			propagate_(true),
			disabled_(false)
		{
		}

		LevelType level_;

		Logger* parent_;

		std::string_view name_;
		bool propagate_;
		bool disabled_;

		mutable std::vector<Logger*> children_;
		mutable std::vector<Sink*> sinks_;
	};

	class LoggerRegistry
	{
		static constexpr std::string_view ROOT_LOGGER_NAME = "root";
		static constexpr auto ROOT_LOGGER_HASH = utility::Hash(ROOT_LOGGER_NAME);
	public:
		LoggerRegistry()
		{
			Logger root(ROOT_LOGGER_NAME);
			loggers_.insert({ROOT_LOGGER_HASH, std::move(root)});
		}

		LoggerRegistry(const LoggerRegistry& other) = delete;
		LoggerRegistry(LoggerRegistry&& other)		= delete;

		LoggerRegistry& operator=(const LoggerRegistry& other) = delete;
		LoggerRegistry& operator=(LoggerRegistry&& other)	   = delete;

		~LoggerRegistry() = default;

		Logger& GetLogger(const std::string_view name)
		{
			const utility::Hash hash(name);
			Logger logger(name);
			return loggers_.try_emplace(hash, std::move(logger)).first->second;
		}

		const Logger& GetRootLogger()
		{
			return loggers_.at(ROOT_LOGGER_HASH);
		}

	private:
		std::unordered_map<utility::Hash, Logger> loggers_;
	};

}

static synodic::honesty::log::LoggerRegistry LOGGER_REGISTRY {};

namespace synodic::honesty::log
{

	export inline const Logger& GetRootLogger()
	{
		return LOGGER_REGISTRY.GetRootLogger();
	}

	export inline Logger& GetLogger(const std::string_view name)
	{
		return LOGGER_REGISTRY.GetLogger(name);
	}
}
