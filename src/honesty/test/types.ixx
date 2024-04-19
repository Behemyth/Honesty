export module synodic.honesty.test:types;

import std;
import function_ref;
import :generator;

namespace synodic::honesty::test
{
	export class TestBase
	{
	public:
		virtual ~TestBase()		 = default;
		virtual void Run() const = 0;

		virtual std::span<std::string_view> Tags() const = 0;

		virtual std::string_view Name() const = 0;

	private:
	};

	export class SuiteData
	{
	public:
		consteval SuiteData(
			std::string_view name,
			std::function_ref<generator<TestBase>()> generator) noexcept;

		std::string_view Name() const noexcept
		{
			return name_;
		}

		generator<TestBase> Generator() const noexcept
		{
			return generator_();
		}

	private:
		std::string_view name_;
		std::function_ref<generator<TestBase>()> generator_;
	};

	consteval SuiteData::SuiteData(
		std::string_view name,
		std::function_ref<generator<TestBase>()> generator) noexcept :
		name_(name),
		generator_(std::move(generator))
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
