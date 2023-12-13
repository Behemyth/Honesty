module synodic.honesty.test:test;

import :test;

namespace synodic::honesty
{
	TestImp& TestImp::operator=(std::move_only_function<void()> test)
	{
		return *this;
	}

	void TestImp::Run()
	{
		runner_();
	}

	TestImp Test(std::string_view name, std::move_only_function<void()> test)
	{
		return TestImp(name, std::move(test));
	}
}
