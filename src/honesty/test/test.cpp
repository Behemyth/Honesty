module synodic.honesty.test;

import std;
import function_ref;
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
	void Test::operator=(std::function_ref<void()> test) const
	{
	}

	Registry& GetRegistry()
	{
		return REGISTRY;
	}

	RunnerContext& GetContext()
	{
		return CONTEXT;
	}
}
