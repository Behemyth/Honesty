export module synodic.honesty.test:types;

import std;
import function_ref;
import synodic.honesty.log;

namespace synodic::honesty::test
{
	export enum class ListOutputType : std::uint8_t
	{
		LOG,
		JSON
	};

	export class AssertException final : public std::exception
	{
	public:
		using std::exception::exception;
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
			explicit AssertionFail(std::source_location location, const bool exception = false) :
				exception(exception),
				location(std::move(location))
			{
			}

			bool exception;	 // True if the remainder of the test is skipped
			std::source_location location;
		};

		struct EqualityFail : AssertionFail
		{
			explicit EqualityFail(
				std::source_location location,
				const bool equal,
				const std::string_view a,
				const std::string_view b,
				const bool exception = false) :
				AssertionFail(std::move(location), exception),
				equal(equal),
				a(a),
				b(b)
			{
			}

			bool equal;

			std::string a;	 // First value
			std::string b;	 // Second value
		};

		struct ComparisonFail : AssertionFail
		{
			explicit ComparisonFail(
				std::source_location location,
				const std::strong_ordering ordering,
				const std::string_view a,
				const std::string_view b,
				const bool exception = false) :
				AssertionFail(std::move(location), exception),
				ordering(ordering),
				a(a),
				b(b)
			{
			}

			std::strong_ordering ordering;	// The ordering that failed between the two values

			std::string a;	 // First value
			std::string b;	 // Second value
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
}
