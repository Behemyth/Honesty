export module synodic.honesty.trace:context;

import std;
import :types;

namespace honesty::trace
{
	/**
	 * @brief Thread-local context for propagating trace information across span boundaries
	 */
	export class Context
	{
	public:
		/**
		 * @brief Get the current thread-local context
		 */
		static Context& Current()
		{
			static thread_local Context instance;
			return instance;
		}

		/**
		 * @brief Get the current trace ID
		 */
		const TraceID& GetTraceID() const { return traceID_; }

		/**
		 * @brief Get the current span ID (parent for new spans)
		 */
		const SpanID& GetSpanID() const { return spanID_; }

		/**
		 * @brief Set the current trace context
		 */
		void SetCurrent(TraceID traceID, SpanID spanID)
		{
			traceID_ = traceID;
			spanID_ = spanID;
		}

		/**
		 * @brief Clear the current context
		 */
		void Clear()
		{
			traceID_ = {};
			spanID_ = {};
		}

		/**
		 * @brief Check if there is an active trace
		 */
		bool IsValid() const
		{
			return traceID_.IsValid();
		}

	private:
		Context() = default;

		TraceID traceID_{};
		SpanID spanID_{};
	};

	/**
	 * @brief RAII guard for span context - restores previous context on destruction
	 */
	export class ContextGuard
	{
	public:
		explicit ContextGuard(TraceID newTraceID, SpanID newSpanID) :
			previousTraceID_(Context::Current().GetTraceID()),
			previousSpanID_(Context::Current().GetSpanID()),
			active_(true)
		{
			Context::Current().SetCurrent(newTraceID, newSpanID);
		}

		~ContextGuard()
		{
			if (active_)
			{
				Context::Current().SetCurrent(previousTraceID_, previousSpanID_);
			}
		}

		ContextGuard(const ContextGuard&) = delete;
		ContextGuard& operator=(const ContextGuard&) = delete;

		ContextGuard(ContextGuard&& other) noexcept :
			previousTraceID_(other.previousTraceID_),
			previousSpanID_(other.previousSpanID_),
			active_(other.active_)
		{
			other.active_ = false;  // Prevent other from restoring context
		}

		ContextGuard& operator=(ContextGuard&&) = delete;

	private:
		TraceID previousTraceID_;
		SpanID previousSpanID_;
		bool active_;
	};

	/**
	 * @brief No-op context guard for disabled tracing
	 */
	export class NoopContextGuard
	{
	public:
		constexpr NoopContextGuard() = default;
	};
}
