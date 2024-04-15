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
			logger_.Log("Suite Begin: {}", event.name);
		}

		void signal(const event::TestBegin& event) override
		{
			logger_.Log("Test Begin: {}", event.name);
		}

		void signal(const event::AssertionPass& event) override
		{
			std::string styledResult = format(log::text_style(log::color24_t(0, 255, 0)), "Passed");
			logger_.Log(
				"Assertion {}: File({}), Line({})",
				styledResult,
				event.location.file_name(),
				event.location.line());
		}

		void signal(const event::AssertionFail& event) override
		{
			std::string styledResult = format(log::text_style(log::color24_t(255, 0, 0)), "Failed");
			logger_.Log("Test {}: File({}), Line({})", styledResult, event.location.file_name(), event.location.line());
		}

	private:
		log::Console consoleSink_;
		log::Logger logger_;
	};

	Console::Console(std::string_view name) :
		StreamingAdapter(name),
		consoleSink_(),
		logger_(log::Logger("reporter", consoleSink_))
	{
	}
}
