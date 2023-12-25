module synodic.honesty.test:test;

import :test;

using namespace synodic::honesty::literals;

namespace synodic::honesty
{
	TestGenerator TestName::operator=(
		TestGenerator (*generator)()) const
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

	TestGenerator Test(std::string_view name, TestGenerator(* generator)())
	{
		for(TestBase&& test : generator())
		{
			// TODO: inject changes
			co_yield Test("", [](){});
		}
	}

	VoidTest Test(std::string_view name, void (*generator)())
	{
		return {name, generator};
	}

	VoidTest& VoidTest::operator=(void (*test)())
	{
		runner_ = test;
		return *this;
	}

	void VoidTest::Run() const
	{
		runner_();
	}
}
