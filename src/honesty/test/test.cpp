module synodic.honesty.test:test;

import :test;

using namespace synodic::honesty::literals;

namespace synodic::honesty
{
	TestGenerator TestName::operator=(std::move_only_function<TestGenerator() const> generator) const
	{
		return Test(name_, std::move(generator));
	}

	VoidTest TestName::operator=(std::move_only_function<void() const> generator) const
	{
		return {name_, std::move(generator)};
	}

	VoidTest::VoidTest(std::string_view name, std::move_only_function<void() const> test) :
		runner_(std::move(test))
	{
	}

	TestGenerator Test(std::string_view name, std::move_only_function<TestGenerator() const> generator)
	{
		for (TestBase&& test: generator())
		{
			// TODO: inject changes
			co_yield Test(
				"",
				[]()
				{
				});
		}
	}

	VoidTest Test(std::string_view name, std::move_only_function<void() const> generator)
	{
		return {name, std::move(generator)};
	}

	VoidTest& VoidTest::operator=(std::move_only_function<void() const> test)
	{
		runner_ = std::move(test);
		return *this;
	}

	void VoidTest::Run() const
	{
		runner_();
	}
}
