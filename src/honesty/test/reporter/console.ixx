export module synodic.honesty.test:reporter.console;

import :reporter;
import :logger;

import std;

namespace synodic::honesty::test::reporter
{
	export template<synodic::honesty::test::logger_type LoggerType>
	class Console : public synodic::honesty::test::StreamingAdapter
	{
	public:
		Console(LoggerType logger);
		~Console() override = default;

		void signal(const event::SuiteBegin& event) override
		{
			logger_.log("Suite Begin: {}", event.name);
		}

		void signal(const event::TestBegin& event) override
		{
			logger_.log("Test Begin: {}", event.name);
		}

		void signal(const event::AssertionPass& event) override
		{
			logger_.log("Assertion Passed: File({}), Line({})", event.location.file_name(), event.location.line());
		}

		void signal(const event::AssertionFail& event) override
		{
			logger_.log("Test Failed: File({}), Line({})", event.location.file_name(), event.location.line());
		}

	private:
		LoggerType logger_;
	};

	template<synodic::honesty::test::logger_type LoggerType>
	Console<LoggerType>::Console(LoggerType logger) :
		logger_(logger)
	{
	}
}
