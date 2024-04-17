export module synodic.honesty.test.reporter.console;

import synodic.honesty.test;
import synodic.honesty.log;

import std;

namespace synodic::honesty::test
{
	export class Console : public StreamingAdapter
	{
	public:
		Console(std::string_view name);
		~Console() override = default;

		void signal(const event::SuiteBegin& event) override
		{
			logger_.Info("Suite Begin: {}", event.name);
		}

		void signal(const event::TestBegin& event) override
		{
			logger_.Info("Test Begin: {}", event.name);
		}

		void signal(const event::AssertionPass& event) override
		{
			std::string styledResult = format(log::text_style(log::color24_t(0, 255, 0)), "Passed");
			logger_.Info(
				"Assertion {}: File({}), Line({})",
				styledResult,
				event.location.file_name(),
				event.location.line());
		}

		void signal(const event::AssertionFail& event) override
		{
			std::string styledResult = format(log::text_style(log::color24_t(255, 0, 0)), "Failed");
			logger_.Info("Test {}: File({}), Line({})", styledResult, event.location.file_name(), event.location.line());
		}

	private:
		log::Console consoleSink_;
		log::Logger logger_;
	};

	Console::Console(std::string_view name) :
		StreamingAdapter(name),
		logger_(log::Logger("reporter", consoleSink_))
	{
	}
}
