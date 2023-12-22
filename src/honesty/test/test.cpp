module synodic.honesty.test:test;

import :test;

namespace synodic::honesty
{
	//std::ranges::elements_of<TestGenerator> TestName::operator=(
	//	TestGenerator (*generator)()) const
	//{
	//	return Test(name_, generator);
	//}

	VoidTest TestName::operator=(void (*generator)()) const
	{
		return {name_, generator};
	}

	//RecursiveTest::RecursiveTest(std::string_view name, TestGenerator (*generator)()):
	//	generator_(std::move(generator()))
	//{
	//}

	VoidTest::VoidTest(std::string_view name, void (*test)()):
		runner_(test)
	{
	}

	//std::ranges::elements_of<TestGenerator> Test(
	//	std::string_view name,
	//	TestGenerator (*generator)())
	//{
	//	return {generator()};
	//}

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
