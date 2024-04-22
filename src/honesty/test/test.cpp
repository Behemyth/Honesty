module synodic.honesty.test;

import std;
import function_ref;
import :types;
import :registry;
import :runner;

using namespace synodic::honesty::test::literals;

namespace synodic::honesty::test
{
	void Test::operator=(std::function_ref<void()> test) const
	{
		// This is the only place where a test registered
		//GetContext()
	}
}
