module synodic.honesty.test:test;

import :test;

namespace synodic::honesty
{
	auto TestName::operator=(Generator (*generator)()) const
	{
		return Test(name_, generator);
	}

	Test::Test(std::string_view name, void (*test)()):
		runner_(test)
	{
	}

	class Test& Test::operator=(void (*test)())
	{
		return *this;
	}

	void Test::Run() const
	{
		runner_();
	}
}
