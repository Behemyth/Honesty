module synodic.honesty.test;

import std;
import :registry;
import :reporter;
import :runner;
import :test;

namespace synodic::honesty::test
{
	result::Execute Interface::Execute(const parameter::Execute& parameters)
	{
		RunnerContext& context = GetContext();

		// Set the context
		context = RunnerContext(reporters_);

		for (Runner* runner: runners_)
		{
			runner->Run(context.broadcaster);
		}

		return {};
	}

	result::List Interface::List(const parameter::List& parameters)
	{
		return {};
	}
}
