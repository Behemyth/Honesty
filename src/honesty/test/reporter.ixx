export module synodic.honesty.test:reporter;

import std;
import function_ref;
import :types;

namespace synodic::honesty::test
{
	export class Reporter
	{
	public:
		explicit(false) constexpr Reporter(std::string_view name);
		virtual ~Reporter() = default;

		virtual void Signal(const event::SuiteBegin& event) = 0;
		virtual void Signal(const event::SuiteEnd& event)	= 0;
		virtual void Signal(const event::SuiteSkip& event)	= 0;

		virtual void Signal(const event::SuiteRun& event)  = 0;
		virtual void Signal(const event::SuiteFail& event) = 0;
		virtual void Signal(const event::SuitePass& event) = 0;

		virtual void Signal(const event::SuiteSummary& event) = 0;

		virtual void Signal(const event::TestBegin& event) = 0;
		virtual void Signal(const event::TestEnd& event)   = 0;
		virtual void Signal(const event::TestSkip& event)  = 0;

		virtual void Signal(const event::TestRun& event)  = 0;
		virtual void Signal(const event::TestFail& event) = 0;
		virtual void Signal(const event::TestPass& event) = 0;

		virtual void Signal(const event::AssertionFail& event) = 0;
		virtual void Signal(const event::AssertionPass& event) = 0;
		virtual void Signal(const event::AssertionSkip& event) = 0;

		virtual void Signal(const event::Summary& event) = 0;

		std::string_view Name() const;

	private:
		std::string_view name_;
	};

	constexpr Reporter::Reporter(std::string_view name) :
		name_(name)
	{
	}

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
		explicit(false) constexpr StreamingAdapter(std::string_view name);
		~StreamingAdapter() override = default;

		void Signal(const event::SuiteBegin& event) override
		{
		}

		void Signal(const event::SuiteEnd& event) override
		{
		}

		void Signal(const event::SuiteSkip& event) override
		{
		}

		void Signal(const event::SuiteRun& event) override
		{
		}

		void Signal(const event::SuiteFail& event) override
		{
		}

		void Signal(const event::SuitePass& event) override
		{
		}

		void Signal(const event::SuiteSummary& event) override
		{
		}

		void Signal(const event::TestBegin& event) override
		{
		}

		void Signal(const event::TestEnd& event) override
		{
		}

		void Signal(const event::TestSkip& event) override
		{
		}

		void Signal(const event::TestRun& event) override
		{
		}

		void Signal(const event::TestFail& event) override
		{
		}

		void Signal(const event::TestPass& event) override
		{
		}

		void Signal(const event::AssertionFail& event) override
		{
		}

		void Signal(const event::AssertionPass& event) override
		{
		}

		void Signal(const event::AssertionSkip& event) override
		{
		}

		void Signal(const event::Summary& event) override
		{
		}
	};

	constexpr StreamingAdapter::StreamingAdapter(const std::string_view name) :
		Reporter(name)
	{
	}
}
