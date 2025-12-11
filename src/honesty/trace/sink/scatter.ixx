export module synodic.honesty.trace:sink.scatter;

import std;
import inplace_vector;

import :types;
import :span;

namespace honesty::trace
{
	/**
	 * @brief Type-erased sink wrapper for heterogeneous sink storage
	 */
	class SinkWrapper
	{
	public:
		template<typename SinkT>
		explicit SinkWrapper(SinkT* sink) :
			sink_(sink),
			exportFn_([](void* s, std::span<const SpanData> spans) {
				static_cast<SinkT*>(s)->Export(spans);
			}),
			flushFn_([](void* s) {
				static_cast<SinkT*>(s)->Flush();
			})
		{
		}

		void Export(std::span<const SpanData> spans) const { exportFn_(sink_, spans); }
		void Flush() const { flushFn_(sink_); }
		void* Raw() const { return sink_; }

	private:
		void* sink_;
		void (*exportFn_)(void*, std::span<const SpanData>);
		void (*flushFn_)(void*);
	};

	/**
	 * @brief Fixed-capacity scatter sink that forwards spans to multiple backends
	 * @tparam N Maximum number of sinks
	 */
	export template<std::size_t N>
	class FixedScatter final
	{
	public:
		FixedScatter() = default;

		void Export(std::span<const SpanData> spans)
		{
			for (const auto& sink : sinks_)
			{
				sink.Export(spans);
			}
		}

		void Flush()
		{
			for (const auto& sink : sinks_)
			{
				sink.Flush();
			}
		}

		/**
		 * @brief Add a sink to forward to. No-op if already present or at capacity.
		 */
		template<typename SinkT>
		void AddSink(SinkT* sink)
		{
			if (!sink || sinks_.size() >= N)
				return;

			for (const auto& existing : sinks_)
			{
				if (existing.Raw() == sink)
					return;
			}
			sinks_.emplace_back(sink);
		}

		/**
		 * @brief Remove a sink from the scatter
		 */
		template<typename SinkT>
		void RemoveSink(SinkT* sink)
		{
			std::erase_if(sinks_, [sink](const SinkWrapper& w) {
				return w.Raw() == sink;
			});
		}

		/**
		 * @brief Get the number of registered sinks
		 */
		std::size_t Size() const { return sinks_.size(); }

	private:
		std::inplace_vector<SinkWrapper, N> sinks_;
	};

	/**
	 * @brief Dynamic scatter sink that forwards spans to multiple backends
	 */
	export class Scatter final
	{
	public:
		Scatter() = default;

		void Export(std::span<const SpanData> spans)
		{
			for (const auto& sink : sinks_)
			{
				sink.Export(spans);
			}
		}

		void Flush()
		{
			for (const auto& sink : sinks_)
			{
				sink.Flush();
			}
		}

		/**
		 * @brief Add a sink to forward to. No-op if already present.
		 */
		template<typename SinkT>
		void AddSink(SinkT* sink)
		{
			if (!sink)
				return;

			for (const auto& existing : sinks_)
			{
				if (existing.Raw() == sink)
					return;
			}
			sinks_.emplace_back(sink);
		}

		/**
		 * @brief Remove a sink from the scatter
		 */
		template<typename SinkT>
		void RemoveSink(SinkT* sink)
		{
			std::erase_if(sinks_, [sink](const SinkWrapper& w) {
				return w.Raw() == sink;
			});
		}

		/**
		 * @brief Get the number of registered sinks
		 */
		std::size_t Size() const { return sinks_.size(); }

	private:
		std::vector<SinkWrapper> sinks_;
	};
}
