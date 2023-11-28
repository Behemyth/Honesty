module synodic.honesty.test:set;

import std;
import :set;
import :suite;
import :runner;

namespace synodic::honesty
{
	Set::Set(std::string_view name) :
		name_(name)
	{
	}

	Set::Set(std::string_view name, std::move_only_function<Generator()>&& generator) :
		name_(name),
		callable_(std::move(generator))
	{
	}

	Set& Set::operator=(std::move_only_function<Generator()>&& generator) noexcept
	{
		callable_ = std::move(generator);
		return *this;
	}
}
