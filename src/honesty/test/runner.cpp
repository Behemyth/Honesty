module synodic.honesty.test;

import std;
import :registry;

namespace synodic::honesty::test
{
	bool RegisterRunner(Runner& runner)
	{
		GetRegistry().AddRunner(runner);
		return true;
	}
}
