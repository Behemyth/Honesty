export module synodic.honesty.test:reporter.default;

import synodic.honesty.log;
import :reporter;
import std;

namespace synodic::honesty::test
{
	export class DefaultReporter final : public StreamingAdapter
	{
	public:
		explicit DefaultReporter(log::Logger logger) :
			StreamingAdapter("default", std::move(logger))
		{
		}

		~DefaultReporter() override = default;

		void Signal(const event::SuiteBegin& event) override
		{
			Logger().Info("Suite Begin: {}", event.name);
		}

		void Signal(const event::TestBegin& event) override
		{
			Logger().Info("Test Begin: {}", event.name);
		}

		void Signal(const event::AssertionPass& event) override
		{
			std::string styledResult = format(log::TextStyle(log::color24_t(0, 255, 0)), "Passed");
			Logger().Info(
				"Assertion {}: File({}), Line({})",
				styledResult,
				event.location.file_name(),
				event.location.line());
		}

		void Signal(const event::AssertionFail& event) override
		{
			std::string styledResult = format(log::TextStyle(log::color24_t(255, 0, 0)), "Failed");
			Logger()
				.Info("Test {}: File({}), Line({})", styledResult, event.location.file_name(), event.location.line());
		}
	};

}
