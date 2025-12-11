export module synodic.honesty.trace:sink;

import std;

import :types;
import :span;

namespace honesty::trace
{
	/**
	 * @brief Concept for mutex types used in synchronized sinks
	 */
	export template<typename T>
	concept mutex = requires(T type) {
		{ type.lock() } -> std::same_as<void>;
		{ type.unlock() } -> std::same_as<void>;
	};

	/**
	 * @brief Base class for trace sinks that export span data to backends
	 */
	export class Sink
	{
	public:
		constexpr Sink() = default;
		virtual ~Sink() = default;

		Sink(const Sink& other) = delete;
		Sink(Sink&& other) noexcept = default;
		Sink& operator=(const Sink& other) = delete;
		Sink& operator=(Sink&& other) noexcept = default;

		/**
		 * @brief Export a batch of span data to the backend
		 * @param spans The span data to export
		 */
		virtual void Export(std::span<const SpanData> spans) = 0;

		/**
		 * @brief Flush any buffered data to the backend
		 */
		virtual void Flush() {}
	};

	/**
	 * @brief Thread-safe sink wrapper using CRTP pattern
	 */
	export template<mutex Mutex>
	class SynchronizedSink : public Sink
	{
	public:
		SynchronizedSink() = default;
		~SynchronizedSink() override = default;

		void Export(std::span<const SpanData> spans) final
		{
			std::lock_guard<Mutex> lock(mutex_);
			SynchronizedExport(spans);
		}

		void Flush() final
		{
			std::lock_guard<Mutex> lock(mutex_);
			SynchronizedFlush();
		}

	protected:
		virtual void SynchronizedExport(std::span<const SpanData> spans) = 0;
		virtual void SynchronizedFlush() {}

		Mutex mutex_;
	};

	/**
	 * @brief A no-op sink for when tracing output is disabled but spans are still created
	 */
	export class NoopSink final : public Sink
	{
	public:
		constexpr NoopSink() = default;

		void Export(std::span<const SpanData> /*spans*/) override {}
		void Flush() override {}
	};
}
