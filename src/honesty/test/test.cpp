module synodic.honesty.test:test;

import :test;

using namespace synodic::honesty::literals;

namespace synodic::honesty
{

	VoidTest::VoidTest(std::string_view name, std::function_ref<void()> test) :
		runner_(std::move(test))
	{
	}

	TestGenerator Test(std::string_view name, std::function_ref<TestGenerator()> generator)
	{
		for (const TestBase& test: generator())
		{
			// TODO: inject changes
			co_yield Test(
				"",
				[]()
				{
				});
		}
	}

	VoidTest Test(std::string_view name, std::function_ref<void()> generator)
	{
		return {name, std::move(generator)};
	}

	VoidTest& VoidTest::operator=(std::function_ref<void()> test)
	{
		runner_ = std::move(test);
		return *this;
	}

	void VoidTest::Run() const
	{
		runner_();
	}
}
