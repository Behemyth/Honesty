export module synodic.honesty.trace:span;

import std;

import :context;
import :zone;

namespace synodic::honesty::trace
{
	export class Span
	{
	public:
		~Span()
		{
			endTime_ = std::chrono::high_resolution_clock::now();
			auto duration = endTime_ - startTime_;

		}

	private:
		friend class Tracer;


		Span(Context& context, const Zone& zone) :
			context_(context), labelID_(0)
		{
			startTime_ = std::chrono::high_resolution_clock::now();
		}

		Span(Context& context, std::string_view label) :
			context_(context), labelID_(0)
		{
		}

		std::reference_wrapper<Context> context_;
		std::chrono::steady_clock::time_point startTime_;
		std::chrono::steady_clock::time_point endTime_;
		std::uint64_t labelID_;
	};
}
