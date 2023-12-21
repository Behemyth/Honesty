module synodic.honesty.test:test;

import :test;

namespace synodic::honesty
{
	std::ranges::elements_of<Generator> TestName::operator=(Generator (*generator)()) const
	{
		return Test(name_, generator);
	}

	VoidTest TestName::operator=(void (*generator)()) const
	{
		return {name_, generator};
	}

	VoidTest::VoidTest(std::string_view name, void (*test)()):
		runner_(test)
	{
	}

	std::ranges::elements_of<Generator> Test(std::string_view name, Generator (*generator)())
	{
		return std::ranges::elements_of(generator());
	}

	VoidTest Test(std::string_view name, void (*generator)())
	{
		return {name, generator};
	}

	VoidTest& VoidTest::operator=(void (*test)())
	{
		return *this;
	}

	void VoidTest::Run() const
	{
		runner_();
	}
}
