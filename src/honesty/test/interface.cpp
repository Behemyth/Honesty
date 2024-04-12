module synodic.honesty.test;

import std;
import :registry;
import :reporter;
import :runner;
import :test;
import :logger;

namespace synodic::honesty::test
{
	ExecuteParameters::ExecuteParameters(Runner* runner, Reporter* reporter) :
		runner(runner),
		reporter(reporter)
	{
	}

	Interface::Interface(const Configuration& configuration)
	{
		Registry& registry = GetRegistry();

		reporters_ = registry.GetReporters();
		runners_ = registry.GetRunners();
	}

	HelpResult Interface::Help(const HelpParameters& parameters)
	{
		return {};
	}

	ExecuteResult Interface::Execute(const ExecuteParameters& parameters)
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

	ListResult Interface::List(const ListParameters& parameters)
	{
		return {};
	}
}
