module synodic.honesty.test:test;

import :test;

namespace synodic::honesty
{
	std::ranges::elements_of<std::generator<TestBase>> TestName::operator=(
		std::generator<TestBase> (*generator)()) const
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

	std::ranges::elements_of<std::generator<TestBase>> Test(
		std::string_view name,
		std::generator<TestBase> (*generator)())
	{
		return {generator()};
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
