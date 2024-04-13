export module synodic.honesty.test:reporter;

import std;
import function_ref;
import :types;

namespace synodic::honesty::test
{
	export class Reporter
	{
	public:
		consteval Reporter(std::string_view name);
		virtual ~Reporter() = default;

		virtual void signal(const event::SuiteBegin& event) = 0;
		virtual void signal(const event::SuiteEnd& event)	= 0;
		virtual void signal(const event::SuiteSkip& event)	= 0;

		virtual void signal(const event::SuiteRun& event)  = 0;
		virtual void signal(const event::SuiteFail& event) = 0;
		virtual void signal(const event::SuitePass& event) = 0;

		virtual void signal(const event::SuiteSummary& event) = 0;

		virtual void signal(const event::TestBegin& event) = 0;
		virtual void signal(const event::TestEnd& event)   = 0;
		virtual void signal(const event::TestSkip& event)  = 0;

		virtual void signal(const event::TestRun& event)  = 0;
		virtual void signal(const event::TestFail& event) = 0;
		virtual void signal(const event::TestPass& event) = 0;

		virtual void signal(const event::AssertionFail& event) = 0;
		virtual void signal(const event::AssertionPass& event) = 0;
		virtual void signal(const event::AssertionSkip& event) = 0;

		virtual void signal(const event::Summary& event) = 0;

		constexpr std::string_view Name() const;

	private:
		std::string_view name_;
	};

	consteval Reporter::Reporter(std::string_view name) :
		name_(name)
	{
	}

	constexpr std::string_view Reporter::Name() const
	{
		return name_;
	}

	/**
	 * @brief TODO: Actually implement this instead of just stubbing it out.
	 */
	export class StreamingAdapter : public Reporter
	{
	public:
		consteval StreamingAdapter(std::string_view name) :
			Reporter(name)
		{
		}

		~StreamingAdapter() override = default;

		void signal(const event::SuiteBegin& event) override {};
		void signal(const event::SuiteEnd& event) override {};
		void signal(const event::SuiteSkip& event) override {};

		void signal(const event::SuiteRun& event) override {};
		void signal(const event::SuiteFail& event) override {};
		void signal(const event::SuitePass& event) override {};

		void signal(const event::SuiteSummary& event) override {};

		void signal(const event::TestBegin& event) override {};
		void signal(const event::TestEnd& event) override {};
		void signal(const event::TestSkip& event) override {};

		void signal(const event::TestRun& event) override {};
		void signal(const event::TestFail& event) override {};
		void signal(const event::TestPass& event) override {};

		void signal(const event::AssertionFail& event) override {};
		void signal(const event::AssertionPass& event) override {};
		void signal(const event::AssertionSkip& event) override {};

		void signal(const event::Summary& event) override {};
	};
}
