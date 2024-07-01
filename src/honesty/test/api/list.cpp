module synodic.honesty.test:api.list;

import std;
import synodic.honesty.log;

import :reporter.list;

namespace synodic::honesty::test::api
{
	struct ListParameters
	{
		explicit ListParameters(const log::Logger& logger) :
			logger(logger)
		{
		}

		std::reference_wrapper<const log::Logger> logger;
	};

	struct ListResult
	{
		ListResult() = default;

		std::vector<SuiteDescription> suites;
	};

	ListResult List(const ListParameters& parameters)
	{
		ListReporterParameters listReporterParameters;
		ListReporter listReporter(listReporterParameters, parameters.logger);

		EmptyRunner runner(log::RootLogger().CreateLogger("empty_runner"));

		Reporter* reporter = &listReporter;
		std::ranges::single_view reporters{reporter};

		Context context(runner, reporters);

		const ExecuteParameters executeParameters(context, "", parameters.logger);
		ExecuteResult executeResult = Execute(executeParameters);

		ListResult result;

		const CumulativeAdapter::CumulativeData& data = listReporter.Data();

		for (const auto& [name, tests]: data.suites)
		{
			auto& resultSuite = result.suites.emplace_back();
			resultSuite.name = name;

			for (const auto& [name]: tests)
			{
				TestDescription description;
				description.name = name;

				resultSuite.tests.push_back(description);
			}
		}

		return result;
	}
}
