export module synodic.honesty.trace:span;

import std;

import synodic.honesty.metric;

import :context;
import :scope;
import :types;

namespace honesty::trace
{
	/**
	 * @brief The collected data of a span. Meant to be a small size and efficient to process in bulk numbers
	 */
	struct SpanData
	{
		honesty::metric::Clock::time_point startTime;
		honesty::metric::Duration duration;
		std::uint32_t spanID;
		std::uint32_t parentSpanID;
		std::uint32_t metadataID;
	};

	/**
     * @brief A no-op span for when tracing is disabled.
     */
	class NoopSpan
	{
	public:
		constexpr explicit NoopSpan() noexcept
		{
		}
	};

	/**
	 * @brief The public interface for recording a span of time
	 */
	export class Span
	{
		template<TracerConfiguration Configuration>
		friend class Tracer;

	public:
		~Span()
		{
			const auto endTime = metric::Clock::now();
			auto duration      = endTime - startTime_;
		}

	private:
		explicit Span(const Scope& scope) :
			startTime_(metric::Clock::now())
		{
		}

		constexpr explicit Span(std::string_view label)
		{
		}

		metric::Clock::time_point startTime_;
	};
}
