export module synodic.honesty.test:types;

import std;
import function_ref;
import inplace_vector;
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

	template<typename T>
	class Registry
	{
	public:
		Registry()
		{
			registrars_.push_back(this);
		}

		virtual ~Registry() = default;

		virtual std::unique_ptr<T> Create(log::Logger logger) = 0;

	private:
		constinit static std::inplace_vector<Registry*, 2> registrars_;
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
			bool exception;	 // True if the remainder of the test is skipped
			std::source_location location;
		};

		struct AssertionPass
		{
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
