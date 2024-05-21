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
			StreamingAdapter(std::move(logger))
		{
		}

		~CompactReporter() override = default;

		consteval std::string_view Name() const
		{
			return "compact";
		}
	};

}
