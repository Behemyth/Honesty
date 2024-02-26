export module synodic.honesty.test:reporter;

import std;
import function_ref;

namespace synodic::honesty::test
{

	export namespace event
	{

		struct SuiteBegin
		{
			std::string_view name;
		};

		struct SuiteEnd
		{
			std::string_view name;
		};

		struct SuiteSkip
		{
			std::string_view name;
		};

		struct SuiteRun
		{
			std::string_view name;
		};

		struct SuiteFail
		{
			std::string_view name;
		};

		struct SuitePass
		{
			std::string_view name;
		};

		struct SuiteSummary
		{
			std::string_view name;
		};

		struct TestBegin
		{
			TestBegin(std::string_view name) :
				name(name)
			{
			}

			std::string_view name;
		};

		struct TestEnd
		{
			std::string_view name;
		};

		struct TestSkip
		{
			std::string_view name;
		};

		struct TestRun
		{
			std::string_view name;
		};

		struct TestFail
		{
			std::string_view name;
		};

		struct TestPass
		{
			std::string_view name;
		};

		struct AssertionFail
		{
			std::string_view name;
		};

		struct AssertionPass
		{
			std::string_view name;
		};

		struct AssertionSkip
		{
			std::string_view name;
		};

		struct Summary
		{
			std::string_view name;
		};
	}

	export class Reporter
	{
	public:
		Reporter()			= default;
		virtual ~Reporter() = default;

		virtual void signal(const event::SuiteBegin& event) {};
		virtual void signal(const event::SuiteEnd& event) {};
		virtual void signal(const event::SuiteSkip& event) {};

		virtual void signal(const event::SuiteRun& event) {};
		virtual void signal(const event::SuiteFail& event) {};
		virtual void signal(const event::SuitePass& event) {};

		virtual void signal(const event::SuiteSummary& event) {};

		virtual void signal(const event::TestBegin& event) {};
		virtual void signal(const event::TestEnd& event) {};
		virtual void signal(const event::TestSkip& event) {};

		virtual void signal(const event::TestRun& event) {};
		virtual void signal(const event::TestFail& event) {};
		virtual void signal(const event::TestPass& event) {};

		virtual void signal(const event::AssertionFail& event) {};
		virtual void signal(const event::AssertionPass& event) {};
		virtual void signal(const event::AssertionSkip& event) {};

		virtual void signal(const event::Summary& event) {};

	private:
	};

	template<class T, class... U>
	concept any_of = std::disjunction_v<std::is_same<T, U>...>;

	export class Events
	{
	public:
		Events(std::span<Reporter* const> reporters);

		template<typename Event>
			requires any_of<
				Event,
				event::SuiteBegin,
				event::SuiteEnd,
				event::SuiteSkip,
				event::SuiteRun,
				event::SuiteFail,
				event::SuitePass,
				event::SuiteSummary,
				event::TestBegin,
				event::TestEnd,
				event::TestSkip,
				event::TestRun,
				event::TestFail,
				event::TestPass,
				event::AssertionFail,
				event::AssertionPass,
				event::AssertionSkip,
				event::Summary>
		void signal(const Event& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->signal(event);
			}
		}

	private:
		std::span<Reporter* const> reporters_;
	};
}
