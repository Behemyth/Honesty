export module synodic.honesty.trace:span;

import std;

import :context;
import :scope;
import :tracer;

namespace honesty::trace
{
	/**
	 * @brief The collected data of a span. Meant to be a small size and efficient to process in bulk numbers
	 */
	struct SpanData
	{
		std::chrono::high_resolution_clock::time_point startTime;
		std::chrono::high_resolution_clock::duration duration;
		std::uint32_t spanID;
		std::uint32_t parentSpanID;
		std::uint32_t metadataID;
	};

	class DisabledSpan
	{
	};

	class EnabledSpan
	{
	};

	/**
	 * @brief The public interface for recording a span of time
	 */
	export class Span
	{
	public:
		~Span()
		{
			const auto endTime = std::chrono::high_resolution_clock::now();

			auto duration = endTime - startTime_;
		}

	private:
		friend class Tracer;

		explicit Span(const Scope& scope) :
			startTime_(std::chrono::high_resolution_clock::now())
		{
		}

		explicit Span(std::string_view label)
		{
		}

		Span(std::string_view label, Tracer tracer)
		{
		}

		std::chrono::high_resolution_clock::time_point startTime_;
	};
}
