
export module synodic.honesty.test.backend:suite;

import :test;
import :generator;

import std;

export namespace synodic::honesty
{
	class SuiteData
	{
	public:
		consteval SuiteData(std::string_view name, std::function_ref<generator<TestBase>()> generator) noexcept;

		std::string_view name_;
		std::function_ref<generator<TestBase>()> generator_;
	};

	consteval SuiteData::SuiteData(std::string_view name, std::function_ref<generator<TestBase>()> generator) noexcept :
		name_(name),
		generator_(generator)
	{
	}

}
