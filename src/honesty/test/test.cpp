module synodic.honesty.test;

import std;
import function_ref;
import :generator;
import :types;
import :registry;
import :runner;

using namespace synodic::honesty::test::literals;

namespace
{
	// The fallback context for tests
	inline synodic::honesty::test::RunnerContext emptyContext({});

	// Each thread has its own context, such that tests can reference global functions without an object
	inline thread_local synodic::honesty::test::RunnerContext& context = emptyContext;

	constinit synodic::honesty::test::Registry registry;
}

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

	Registry& GetRegistry()
	{
		return registry;
	}

	RunnerContext& GetContext()
	{
		return context;
	}

	bool RegisterRunner(Runner& runner)
	{
		registry.AddRunner(runner);
		return true;
	}

	bool RegisterReporter(Reporter& reporter)
	{
		registry.AddReporter(reporter);
		return true;
	}

	void assert(bool expression, const std::source_location& location)
	{
		if (expression)
		{
			event::AssertionPass passed;
			passed.location = location;

			context.broadcaster.signal(passed);
		}
		else
		{
			event::AssertionFail failed;
			failed.location = location;

			context.broadcaster.signal(failed);

			// TODO: Quit the specific test
		}
	}

	bool expect(bool expression, const std::source_location& location)
	{
		if (expression)
		{
			event::AssertionPass passed;
			passed.location = location;

			context.broadcaster.signal(passed);
		}
		else
		{
			event::AssertionFail failed;
			failed.location = location;

			context.broadcaster.signal(failed);
		}

		return expression;
	}
}
