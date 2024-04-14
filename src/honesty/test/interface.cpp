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
		auto suites = GetRegistry().GetSuites();
		parameters.runner->Submit(suites);

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
		auto suites = GetRegistry().GetSuites();

		ListResult result;

		for (const SuiteData* const suite: suites)
		{
			auto generator = suite->Generator();
			for (const TestBase& test: generator())
			{
				TestDescription description;
				description.name = test.Name();

				result.tests.push_back(description);
			}
		}

		return result;
	}
}
