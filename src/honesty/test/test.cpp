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
		try
		{
			GetContext().Run(test);
		}
		catch (const AssertException& exception)
		{
		}
	}
}
