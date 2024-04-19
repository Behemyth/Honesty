module synodic.honesty.test;

import std;
import :registry;
import :reporter;
import :runner;
import :test;

namespace synodic::honesty::test
{
	ExecuteParameters::ExecuteParameters(Runner* runner, Reporter* reporter) :
		runner(runner),
		reporter(reporter)
	{
	}

	ListParameters::ListParameters() :
		outputType(ListOutputType::LOG)
	{
	}

	Interface::Interface(const Configuration& configuration)
	{
	}

	HelpResult Interface::Help(const HelpParameters& parameters)
	{
		return {};
	}

	ExecuteResult Interface::Execute(const ExecuteParameters& parameters)
	{
		auto suites = GetRegistry().GetSuites();

		RunnerContext& context = GetContext();
		std::ranges::single_view reporters{parameters.reporter};

		// Lets a runner get the context with proper setup
		auto generateContext = [&]() -> RunnerContext& {
			context = RunnerContext(reporters);
			return context;
		};

		parameters.runner->Run(generateContext);

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
