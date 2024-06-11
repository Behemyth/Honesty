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
			explicit AssertionFail(
				std::source_location location,
				const bool exception		  = false,
				std::string reason = "") :
				exception(exception),
				location(std::move(location)),
				reason(std::move(reason))
			{
			}

			bool exception;	 // True if the remainder of the test is skipped
			std::source_location location;

			// TODO: Prevent allocation
			std::string reason;
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
