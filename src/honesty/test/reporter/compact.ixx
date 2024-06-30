export module synodic.honesty.test:reporter.compact;

import synodic.honesty.log;
import :reporter;
import std;

namespace synodic::honesty::test
{
	export class CompactReporter final : public StreamingAdapter
	{
	public:
		explicit CompactReporter(const log::Logger& logger) :
			StreamingAdapter(logger)
		{
		}

		~CompactReporter() override = default;

		static consteval std::string_view Name()
		{
			return "compact";
		}
	};

}
