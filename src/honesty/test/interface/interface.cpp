module synodic.honesty.test:interface;

import std;
import :interface;
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
