module synodic.honesty.test;

import std;
import :registry;
import :reporter;
import :runner;

namespace synodic::honesty::test
{
	result::Execute Interface::Execute(const parameter::Execute& parameters)
	{
		Broadcast broadcaster;

		for (Runner* runner: runners_)
		{
			runner->Run(broadcaster);
		}

		return {};
	}

	result::List Interface::List(const parameter::List& parameters)
	{
		return {};
	}
}
