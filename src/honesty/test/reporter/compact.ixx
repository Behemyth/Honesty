export module synodic.honesty.test:reporter.compact;

import synodic.honesty.log;
import :reporter;
import std;

namespace synodic::honesty::test
{
	export class CompactReporter final : public StreamingAdapter
	{
	public:
		explicit CompactReporter(log::Logger logger) :
			StreamingAdapter("compact", std::move(logger))
		{
		}

		~CompactReporter() override = default;
	};

}
