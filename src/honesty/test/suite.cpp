module synodic.honesty.test:suite;

import std;

import :suite;

namespace synodic::honesty
{
	std::span<suite> suite_registrar::suites()
	{
		return suites_;
	}

	suite::suite(std::string_view name, std::move_only_function<Generator()> generator) :
		name_(name),
		generator_(std::move(generator))
	{
		suites_.push_back(std::move(*this));
	}
}
