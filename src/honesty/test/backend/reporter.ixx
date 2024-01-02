export module synodic.honesty.test.backend:reporter;

import :logger;
import std;

export namespace synodic::honesty
{

	struct TestBeginEvent
	{
		std::string_view name;
	};

	struct TestEndEvent
	{
		std::string_view name;
	};

	struct TestSkipEvent
	{
		std::string_view name;
	};

	struct TestRunEvent
	{
		std::string_view name;
	};

	struct TestFailEvent
	{
		std::string_view name;
	};

	struct TestPassEvent
	{
		std::string_view name;
	};

	struct AssertionFailEvent
	{
		std::string_view name;
	};

	struct AssertionPassEvent
	{
		std::string_view name;
	};

	struct AssertionSkipEvent
	{
		std::string_view name;
	};

	struct SummaryEvent
	{
		std::string_view name;
	};

	template<logger_type Logger>
	class Reporter
	{
   
	};
}