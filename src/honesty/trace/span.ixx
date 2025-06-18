export module synodic.honesty.trace:span;

import std;

import :context;
import :scope;

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

	/**
	 * @brief The public interface for recording a span of time
	 */
	export class Span
	{
	public:
		explicit Span(const Scope& scope) :
			startTime_(std::chrono::high_resolution_clock::now())
		{
		}

		constexpr explicit Span(std::string_view label)
		{
		}

		~Span()
		{
			const auto endTime = std::chrono::high_resolution_clock::now();

			auto duration = endTime - startTime_;
		}

	private:
		std::chrono::high_resolution_clock::time_point startTime_;
	};
}
