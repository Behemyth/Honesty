module synodic.honesty.test:set;

import std;
import :set;
import :suite;
import :runner;

namespace synodic::honesty
{
	Set::Set(std::string_view name)
	{
	}

	Set::Set(std::string_view name, std::move_only_function<Generator()>&& generator)
	{
	}

	Set& Set::operator=(std::move_only_function<Generator()>&& generator) noexcept
	{
		return *this;
	}

	Set::operator Generator()
	{
		return callable_();
	}
}
