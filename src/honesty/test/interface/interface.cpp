module synodic.honesty.test:interface;

import std;
import :interface;
namespace synodic::honesty::test
{
	void API::Execute(const parameters::Execute& parameters)
	{
		Broadcast broadcaster;

		for (Runner* runner: runners_)
		{
			runner->Run(broadcaster);
		}
	}

	void API::List(const parameters::List& parameters)
	{
	}
}
