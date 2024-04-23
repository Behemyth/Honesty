module synodic.honesty.test;

import std;
import function_ref;
import :types;
import :registry;
import :runner;
import :context;

using namespace synodic::honesty::test::literals;

namespace synodic::honesty::test
{
	void Test::operator=(const std::function_ref<void()> test) const
	{
		// Prior to sending the test to a runner, we need to update the context
		GetContext().Run(test);
	}
}
