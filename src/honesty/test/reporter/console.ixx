export module synodic.honesty.test:reporter.console;

import :reporter;
import :logger;

import std;

namespace synodic::honesty::test::reporter
{
	export template<synodic::honesty::test::logger_type LoggerType>
	class Console : public synodic::honesty::test::Reporter
	{
	public:
		Console(LoggerType logger);

		void signal(const event::SuiteBegin& event) override
		{
			logger_.log("Suite Begin: {}", event.name);
		}

		void signal(const event::TestBegin& event) override
		{
			logger_.log("Test Begin: {}", event.name);
		}

		//template<class... Args>
		//void expect(bool value, const std::source_location location, std::format_string<Args...> fmt, Args&&... args)
		//{
		//	if (!value)
		//	{
		//		logger_.log(stderr, "Test Failed: File({}), Line({})", location.file_name(), location.line());
		//		logger_.log(stderr, fmt, std::forward<Args>(args)...);
		//	}
		//}

		//template<class T, class U>
		//	requires std::equality_comparable_with<T, U>
		//void expect_equals(const T& a, const U& b, const std::source_location& location)
		//{
		//	expect(a == b, location, "{} was expected to equal {}. It did not", a, b);
		//}

	private:
		LoggerType logger_;
	};

	template<synodic::honesty::test::logger_type LoggerType>
	Console<LoggerType>::Console(LoggerType logger) :
		logger_(logger)
	{
	}
}
