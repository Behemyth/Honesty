export module synodic.honesty.log:logger;

import std;
import :colour;
import :sink;
import :types;

namespace synodic::honesty::log
{

	class Logger
	{
	public:
		Logger(const Logger& other)							 = delete;
		constexpr Logger(Logger&& other) noexcept			 = default;
		Logger& operator=(const Logger& other)				 = delete;
		constexpr Logger& operator=(Logger&& other) noexcept = default;

		auto operator<=>(const Logger&) const = default;

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
			for (Sink* sink: Sinks())
			{
				sink->LogV(level, fmt, args);
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
		std::span<Logger*> Children() const;

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

	protected:
		/***
		 * @brief Constructs a logger with the given name. Forces inheritance for construction
		 * @param name The name of the logger
		 */
		explicit(false) constexpr Logger(std::string_view name) :
			level_(LevelType::DEFER),
			parent_(nullptr),
			name_(name),
			propagate_(true),
			disabled_(false)
		{
		}

	private:
		LevelType level_;

		Logger* parent_;

		std::string name_;
		bool propagate_;
		bool disabled_;
	};

	/**
	 * @brief TODO: comment
	 */
	export template<std::size_t SinkSize>
	class StaticLogger final : public Logger
	{
	public:
		explicit(false) constexpr StaticLogger(const std::string_view name) :
			Logger(name)
		{
		}

		std::span<Sink*> Sinks() const
		{
			return sinks_;
		}

		std::span<Logger*> Children() const
		{
			return {};
		}

	private:
		std::array<Sink*, SinkSize> sinks_;
	};

	/**
	 * @brief TODO: comment
	 */
	export class DynamicLogger final : public Logger
	{
	public:
		// TODO:  Hide constructor
		explicit(false) DynamicLogger(const std::string_view name) :
			Logger(name)
		{
		}

		~DynamicLogger() = default;

		DynamicLogger(const DynamicLogger& other)				 = delete;
		DynamicLogger(DynamicLogger&& other) noexcept			 = default;
		DynamicLogger& operator=(const DynamicLogger& other)	 = delete;
		DynamicLogger& operator=(DynamicLogger&& other) noexcept = default;

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

		std::span<Sink*> Sinks() const
		{
			return sinks_;
		}

		std::span<Logger*> Children() const
		{
			return children_;
		}

	private:
		mutable std::vector<Logger*> children_;
		mutable std::vector<Sink*> sinks_;
	};

	export const DynamicLogger& GetRootLogger();
	export DynamicLogger& GetLogger(std::string_view name);
}

namespace
{
	std::map<std::size_t, synodic::honesty::log::DynamicLogger> LOGGERS;
	const auto ROOT_LOGGER_NAME = "root";
	const synodic::honesty::log::DynamicLogger& ROOT_LOGGER =
		LOGGERS.try_emplace(std::hash<std::string_view> {}(ROOT_LOGGER_NAME), ROOT_LOGGER_NAME).first->second;
}

namespace synodic::honesty::log
{

	const DynamicLogger& GetRootLogger()
	{
		return ROOT_LOGGER;
	}

	DynamicLogger& GetLogger(std::string_view name)
	{
		return LOGGERS.try_emplace(std::hash<std::string_view> {}(name), name).first->second;
	}
}
