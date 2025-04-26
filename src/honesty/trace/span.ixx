export module synodic.honesty.trace:span;

import std;

import :context;
import :zone;

namespace honesty::trace
{
	/**
	 * @brief The collected data of a span. Meant to be a small size and efficient to process in bulk numbers
	 */
	struct SpanData
	{
		std::chrono::high_resolution_clock::time_point startTime;
		std::chrono::high_resolution_clock::duration duration;
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

			//context_->
		}

	private:
		friend class Tracer;


		Span(Context& context, const Scope& zone) :
			context_(context), labelID_(0)
		{
			startTime_ = std::chrono::high_resolution_clock::now();
		}

		Span(Context& context, std::string_view label) :
			context_(context), labelID_(0)
		{
		}

		std::reference_wrapper<Context> context_;
		std::chrono::high_resolution_clock::time_point startTime_;
		std::uint64_t labelID_;
	};
}
