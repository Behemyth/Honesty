module synodic.honesty.test:test;

import :test;
import :backend;

using namespace synodic::honesty::test::literals;

namespace synodic::honesty::test
{
	VoidTest::VoidTest(std::string_view name, std::function_ref<void()> test) :
		runner_(std::move(test)),
		name_(name)
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

	std::string_view VoidTest::Name() const
	{
		return name_;
	}

	bool expect(const bool expression, const std::source_location& location)
	{
		if (expression)
		{
			event::AssertionPass passed;
			passed.location = location;

			Registry::Context().broadcaster.signal(passed);
		}
		else
		{
			event::AssertionFail failed;
			failed.location = location;

			Registry::Context().broadcaster.signal(failed);
		}

		return expression;
	}
}
