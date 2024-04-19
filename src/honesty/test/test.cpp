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
	inline synodic::honesty::test::RunnerContext EMPTY_CONTEXT({});

	// Each thread has its own context, such that tests can reference global functions without an object
	inline thread_local synodic::honesty::test::RunnerContext& CONTEXT = EMPTY_CONTEXT;

	constinit synodic::honesty::test::Registry REGISTRY;
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
		return REGISTRY;
	}

	RunnerContext& GetContext()
	{
		return CONTEXT;
	}

	bool RegisterRunner(Runner& runner)
	{
		REGISTRY.AddRunner(runner);
		return true;
	}

	bool RegisterReporter(Reporter& reporter)
	{
		REGISTRY.AddReporter(reporter);
		return true;
	}
}
