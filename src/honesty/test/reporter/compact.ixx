export module synodic.honesty.test:reporter.compact;

import synodic.honesty.log;
import :reporter;
import std;

namespace synodic::honesty::test
{
	export class CompactReporter final : public StreamingAdapter
	{
	public:
		explicit(false) CompactReporter() :
			StreamingAdapter("default"),
			logger_(log::GetLogger("reporter"))
		{
			logger_.AddSink(&consoleSink_);
		}

		~CompactReporter() override = default;

	private:
		log::Console consoleSink_;
		log::Logger& logger_;
	};

}
