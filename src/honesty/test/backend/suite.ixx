
export module synodic.honesty.test.backend:suite;

import :test;
import :generator;

import std;

export namespace synodic::honesty
{
	struct suite_data
	{
		suite_data(std::string_view name, std::function_ref<generator<TestBase>()> generator) noexcept;

		std::string_view name_;
		std::function_ref<generator<TestBase>()> generator_;
	};

	suite_data::suite_data(std::string_view name, std::function_ref<generator<TestBase>()> generator) noexcept :
		name_(name),
		generator_(generator)
	{
	}

}
