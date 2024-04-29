export module synodic.honesty.test:reporter.compact;

import synodic.honesty.log;
import :reporter;
import std;

namespace synodic::honesty::test
{
	export class CompactReporter final : public StreamingAdapter
	{
	public:
		explicit(false) constexpr CompactReporter(const std::string_view name) :
			StreamingAdapter(name),
			logger_("reporter", &consoleSink_)
		{
		}

		~CompactReporter() override = default;

	private:
		log::Console consoleSink_;
		log::StaticLogger<1> logger_;
	};

}
