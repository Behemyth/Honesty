module synodic.honesty.test:test;

import :test;

using namespace synodic::honesty::literals;

namespace synodic::honesty
{

	VoidTest::VoidTest(std::string_view name, std::function_ref<void()> test) :
		runner_(std::move(test))
	{
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
