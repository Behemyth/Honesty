export module synodic.honesty.test:types;

import std;
import function_ref;

namespace synodic::honesty::test
{

	export class SuiteData
	{
	public:
		consteval SuiteData(std::string_view name, std::function_ref<void()> generator) noexcept;

		std::string_view Name() const noexcept;
		void Execute() const noexcept;

	private:
		std::string_view name_;
		std::function_ref<void()> callback_;
	};

	std::string_view SuiteData::Name() const noexcept
	{
		return name_;
	}

	void SuiteData::Execute() const noexcept
	{
		return callback_();
	}

	consteval SuiteData::SuiteData(std::string_view name, std::function_ref<void()> generator) noexcept :
		name_(name),
		callback_(std::move(generator))
	{
	}

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

	class Assert final : public std::exception
	{
	public:
		using std::exception::exception;
	};
}
