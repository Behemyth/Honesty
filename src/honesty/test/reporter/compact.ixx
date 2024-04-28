export module synodic.honesty.test:reporter.compact;

import synodic.honesty.log;
import :reporter;
import std;

namespace synodic::honesty::test
{
	export class CompactReporter : public StreamingAdapter
	{
	public:
		explicit(false) constexpr CompactReporter(std::string_view name);
		~CompactReporter() override = default;

		void Signal(const event::SuiteBegin& event) override
		{
			logger_.Info("Suite Begin: {}", event.name);
		}

		void Signal(const event::TestBegin& event) override
		{
			logger_.Info("Test Begin: {}", event.name);
		}

		void Signal(const event::AssertionPass& event) override
		{
			std::string styledResult = format(log::TextStyle(log::color24_t(0, 255, 0)), "Passed");
			logger_.Info(
				"Assertion {}: File({}), Line({})",
				styledResult,
				event.location.file_name(),
				event.location.line());
		}

		void Signal(const event::AssertionFail& event) override
		{
			std::string styledResult = format(log::TextStyle(log::color24_t(255, 0, 0)), "Failed");
			logger_.Info("Test {}: File({}), Line({})", styledResult, event.location.file_name(), event.location.line());
		}

	private:
		log::Console consoleSink_;
		log::StaticLogger<1> logger_;
	};

	constexpr CompactReporter::CompactReporter(const std::string_view name) :
		StreamingAdapter(name),
		logger_("reporter", &consoleSink_)
	{
	}
}
