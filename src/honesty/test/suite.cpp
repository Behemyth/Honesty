module synodic.honesty.test:suite;

import std;

import :suite;

namespace synodic::honesty
{
	std::vector<suite_data> suite_registrar::suites_{};

	suite_data::suite_data(std::string_view name, std::move_only_function<Generator()> generator) :
		name_(name),
		generator_(std::move(generator))
	{
	}

	void suite_registrar::execute()
	{
		//for (suite& suite: suites_)
		//{
		//	suite.generator_();
		//}
	}

	suite::suite(std::string_view name, std::move_only_function<Generator()> generator)
	{
		suites_.emplace_back(name, std::move(generator));
	}
}
