export module synodic.honesty.test.types:reporter;

import synodic.honesty.log;

import std;

import function_ref;
import inplace_vector;

namespace synodic::honesty::test
{
	export enum class ExpectedAssertOutcome
	{
		PASS,  // The assertion is expected to pass
		FAIL,  // The assertion is expected to fail
		TODO  // The assertion outcome is undefined
	};

	export enum class ExpectedTestOutcome
	{
		PASS,  // The test is expected to pass
		FAIL,  // The test is expected to fail
		TODO,  // The test outcome is undefined
		SKIP  // The test is expected to be skipped
	};

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
			TestBegin(const std::string_view name, const ExpectedAssertOutcome outcome) :
				name(name),
				outcome(outcome)
			{
			}

			std::string_view name;
			ExpectedAssertOutcome outcome;
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
			explicit AssertionFail(
				std::source_location location,
				const std::string_view message,
				const bool exception) :
				exception(exception),
				message(message),
				location(std::move(location))
			{
			}

			bool exception;	 // True if the remainder of the test is skipped
			std::string_view message;
			std::source_location location;
		};

		struct EqualityFail : AssertionFail
		{
			explicit EqualityFail(
				std::source_location location,
				const bool equal,
				const std::string_view a,
				const std::string_view b,
				const std::string_view message,
				const bool exception) :
				AssertionFail(std::move(location), message, exception),
				equal(equal),
				a(a),
				b(b)
			{
			}

			bool equal;

			std::string a;	// First value
			std::string b;	// Second value
		};

		struct ComparisonFail : AssertionFail
		{
			explicit ComparisonFail(
				std::source_location location,
				const std::strong_ordering ordering,
				const std::string_view a,
				const std::string_view b,
				const std::string_view message,
				const bool exception) :
				AssertionFail(std::move(location), message, exception),
				ordering(ordering),
				a(a),
				b(b)
			{
			}

			std::strong_ordering ordering;	// The ordering that failed between the two values

			std::string a;	// First value
			std::string b;	// Second value
		};

		struct AssertionPass

		{
			explicit AssertionPass(std::source_location location) :
				location(std::move(location))
			{
			}

			std::source_location location;
		};

		struct AssertionSkip
		{
		};

		struct Summary
		{
		};
	}

	export class Reporter
	{
	public:
		/**
		 * @brief Constructs a Reporter object
		 * @param logger The logger to associate with this reporter
		 */
		explicit constexpr Reporter(const log::Logger& logger) :
			logger_(logger)
		{
		}

		virtual ~Reporter() = default;

		virtual void Signal(const event::SuiteBegin& event) = 0;
		virtual void Signal(const event::SuiteEnd& event)	= 0;

		virtual void Signal(const event::TestBegin& event) = 0;
		virtual void Signal(const event::TestEnd& event)   = 0;

		virtual void Signal(const event::TestSkip& event) = 0;

		virtual void Signal(const event::AssertionFail& event)	= 0;
		virtual void Signal(const event::EqualityFail& event)	= 0;
		virtual void Signal(const event::ComparisonFail& event) = 0;

		virtual void Signal(const event::AssertionPass& event) = 0;
		virtual void Signal(const event::AssertionSkip& event) = 0;

		virtual void Signal(const event::Summary& event) = 0;

		const log::Logger& Logger() const
		{
			return logger_;
		}

	private:
		std::string_view name_;
		std::reference_wrapper<const log::Logger> logger_;
	};
}
