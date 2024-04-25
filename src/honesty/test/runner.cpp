module synodic.honesty.test;

import std;
import :registry;
import :runner.default;

namespace
{
	constinit synodic::honesty::test::DefaultRunner DEFAULT_RUNNER("default");

	constinit std::array<synodic::honesty::test::Runner*, 1> RUNNERS = {&DEFAULT_RUNNER};
}

namespace synodic::honesty::test
{
	bool RegisterRunner(Runner& runner)
	{
		GetRegistry().AddRunner(runner);
		return true;
	}
}

std::span<synodic::honesty::test::Runner*> GetBuiltinRunners()
{
	return RUNNERS;
}

const synodic::honesty::test::Runner& GetDefaultRunner()
{
	return DEFAULT_RUNNER;
}
