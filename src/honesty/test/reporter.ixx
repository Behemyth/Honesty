export module synodic.honesty.test:reporter;

import std;
import function_ref;
import :types;

namespace synodic::honesty::test
{
	export class Reporter
	{
	public:
		Reporter(std::string_view name);
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

		std::string_view Name() const;

	private:
		std::string_view name_;
	};

	/***
	 * @brief Registers a reporter with the test framework
	 * @param reporter The reporter to register
	 * @return Whether the reporter was successfully registered
	 */
	export bool RegisterReporter(Reporter& reporter);

	/**
	 * @brief TODO: Actually implement this instead of just stubbing it out.
	 */
	export class StreamingAdapter : public Reporter
	{
	public:
		StreamingAdapter(std::string_view name);
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
