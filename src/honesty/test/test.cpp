module synodic.honesty.test:test;

import :test;

using namespace synodic::honesty::literals;

namespace synodic::honesty
{
	TestGenerator TestName::operator=(std::function_ref<TestGenerator()> generator) const
	{
		return TestRecurse(name_, std::move(generator));
	}

	TestGenerator TestName::operator=(TestGenerator generator) const
	{
		return generator;
	}

	VoidTest TestName::operator=(std::function_ref<void()> generator) const
	{
		return {name_, std::move(generator)};
	}

	VoidTest::VoidTest(std::string_view name, std::function_ref<void()> test) :
		runner_(std::move(test))
	{
	}

	TestGenerator TestRecurse(std::string_view name, std::function_ref<TestGenerator()> generator)
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
