module synodic.honesty.test:api.list;

import std;
import synodic.honesty.log;

import :api.types;
import :api.execute;

import :reporter.list;
import :suite;

namespace synodic::honesty::test::api
{
	struct ListParameters
	{
		explicit ListParameters(const std::string_view applicationName, Runner& runner, const log::Logger& logger) :
			applicationName(applicationName),
			runner(runner),
			logger(logger)
		{
		}

		std::string_view applicationName;

		std::reference_wrapper<Runner> runner;
		std::span<std::unique_ptr<Reporter>> reporters;

		std::reference_wrapper<const log::Logger> logger;
	};

	struct ListResult
	{
		ListResult() = default;

		std::vector<SuiteDescription> suites;
	};

	auto List(const ListParameters& parameters) -> ListResult
	{
		// Register the list reporters and immediately grab it from the registration list
		{
			static ReporterRegistrar<ListReporter> listReporterRegistrar;
		}

		// TODO: Filter with list command
		const ExecuteParameters executeParameters(
			parameters.applicationName,
			"",
			parameters.runner,
			parameters.reporters,
			parameters.logger);

		ExecuteResult executeResult = Execute(executeParameters);

		ListResult result;

		// Grab the list reporters and extract the data
		// ListReporter reporters = parameters.reporters.front();

		// const CumulativeAdapter::CumulativeData& data = listReporter.Data();

		// for (const auto& [name, tests]: data.suites)
		//{
		//	auto& resultSuite = result.suites.emplace_back();
		//	resultSuite.name = name;

		//	for (const auto& [name]: tests)
		//	{
		//		TestDescription description;
		//		description.name = name;

		//		resultSuite.tests.push_back(description);
		//	}
		//}

		return result;
	}
}
