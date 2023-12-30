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

	std::span<std::string_view> VoidTest::Tags() const
	{
		return {};
	}

	bool Suite::Register() const
	{
		Registry::Add(suite_data(name_, generator_));

		return true;
	}
}
