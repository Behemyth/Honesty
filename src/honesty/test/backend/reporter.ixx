export module synodic.honesty.test.backend:reporter;

import :logger;
import std;
import function_ref;

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

	class Reporter
	{
	public:
		Reporter()			= default;
		virtual ~Reporter() = default;

		virtual void signal(const TestBeginEvent& event) {};
		virtual void signal(const TestEndEvent& event) {};
		virtual void signal(const TestSkipEvent& event) {};

		virtual void signal(const TestRunEvent& event) {};
		virtual void signal(const TestFailEvent& event) {};
		virtual void signal(const TestPassEvent& event) {};

		virtual void signal(const AssertionFailEvent& event) {};
		virtual void signal(const AssertionPassEvent& event) {};
		virtual void signal(const AssertionSkipEvent& event) {};

		virtual void signal(const SummaryEvent& event) {};

	private:
	};

	class Events
	{
	public:
		Events(std::span<const Reporter* const>);

		template<typename Event>
			requires std::disjunction_v<
				Event,
				TestBeginEvent,
				TestEndEvent,
				TestSkipEvent,
				TestRunEvent,
				TestFailEvent,
				TestPassEvent,
				AssertionFailEvent,
				AssertionPassEvent,
				AssertionSkipEvent,
				SummaryEvent>
		void signal(const Event& event)
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->signal(event);
			}
		}

	private:
		std::span<const Reporter* const> reporters_;
	};
}
