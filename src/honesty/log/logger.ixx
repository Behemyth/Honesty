export module synodic.honesty.log:logger;

import std;
import :colour;
import :sink;
import :types;

namespace synodic::honesty::log
{

	export class Logger
	{
	public:
		explicit(false) Logger(std::string_view name);
		~Logger() = default;

		Logger(const Logger& other)				   = delete;
		Logger(Logger&& other) noexcept			   = default;
		Logger& operator=(const Logger& other)	   = delete;
		Logger& operator=(Logger&& other) noexcept = default;

		auto operator<=>(const Logger&) const = default;

		template<class... Args>
		inline void Log(LevelType level, std::format_string<Args...> fmt, Args&&... args) const
		{
			LogV(level, fmt.get(), std::make_format_args(args...));
		}

		/***
		 * @brief Logs a message with the given level. This is the internal function that all other log functions call
		 * @param level The level of the message
		 * @param fmt The format string
		 * @param args The arguments
		 */
		void LogV(LevelType level, std::string_view fmt, std::format_args args) const;

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
		std::string_view Name() const;

		/**
		 * @brief Returns the level of this logger
		 * @return The level
		 */
		LevelType Level() const;

		/**
		 * @brief Returns the level or the first ancestor that isn't DEFER
		 * @return The effective level
		 */
		LevelType EffectiveLevel() const;

		/**
		 * @brief Sets the level of this logger
		 * @param level The new level
		 */
		void SetLevel(LevelType level);

		/**
		 * @brief Adds a sink to this logger
		 * @param sink The sink to add
		 */
		void AddSink(Sink* sink) const;

		/**
		 * @brief Removes a sink from this logger
		 * @param sink The sink to remove
		 */
		void RemoveSink(Sink* sink) const;

		/**
		 * @brief Returns the sinks of this logger
		 * @return The sinks
		 */
		std::span<Sink*> Sinks() const;

		/**
		 * @brief Checks to see if this logger or its ancestors have any sinks
		 * @return Whether sinks were found
		 */
		bool HasSink() const;

		/**
		 * @brief Returns the parent logger
		 * @return The parent logger
		 */
		Logger* Parent() const;

		/**
		 * @brief Returns the direct children of this logger
		 * @return The children
		 */
		std::span<Logger*> GetChildren() const;

		/**
		 * @brief Checks if this logger is propagating messages
		 * @return Whether propagation is enabled
		 */
		bool Propagate() const;

		/**
		 * @brief Checks if this logger is disabled
		 * @return Whether the logger is disabled
		 */
		bool Disabled() const;

		/**
		 * @brief Sets whether this logger should propagate messages
		 * @param propagate Whether to propagate
		 */
		void SetPropagate(bool propagate);

		/**
		 * @brief Sets whether this logger is disabled
		 * @param disabled Whether to disable the logger
		 */
		void SetDisabled(bool disabled);

	private:
		// Root constructor
		Logger();

		LevelType level_;

		Logger* parent_;
		mutable std::vector<Logger*> children_;

		std::string name_;
		bool propagate_;
		bool disabled_;
		mutable std::vector<Sink*> sinks_;
	};

	export const Logger& GetRootLogger();
	export Logger& GetLogger(std::string_view name);

}
