export module synodic.honesty.test:interface;

import synodic.honesty.log;
import std;
import :suite;
import :context;
import :reporter;
import :runner;
import :test;
import :types;
import :reporter.list;

namespace synodic::honesty::test
{
	export enum class TestResultCode : std::uint8_t
	{
		FAILURE
	};

	export struct HelpParameters
	{
		HelpParameters() = default;
	};

	export struct ExecuteParameters
	{
		ExecuteParameters(const Context& context) :
			context(context)
		{
		}

		Context context;
	};

	export struct ListParameters
	{
		ListParameters(log::Logger logger) :
			logger(std::move(logger))
		{
		}

		log::Logger logger;
		ListOutputType outputType;
	};

	export struct HelpResult
	{
		HelpResult() = default;
	};

	export struct ExecuteResult
	{
		ExecuteResult() = default;
	};

	export struct TestDescription
	{
		TestDescription() = default;

		std::string name;
	};

	export struct SuiteDescription
	{
		SuiteDescription() = default;

		std::string name;
		std::vector<TestDescription> tests;
	};

	export struct ListResult
	{
		ListResult() = default;

		std::vector<SuiteDescription> suites;
	};

	/**
	 * @brief The interface to interacting with the test framework. Multiple interfaces are allowed to exist for the
	 *	same instance.
	 */
	export class Interface
	{
	public:
		struct Configuration
		{
			Configuration() = default;
		};

		explicit Interface(const Configuration& configuration)
		{
		}

		HelpResult Help(const HelpParameters& parameters)
		{
			return {};
		}

		ExecuteResult Execute(const ExecuteParameters& parameters)
		{

			for (const SuiteView& suite: GetSuites())
			{
				event::SuiteBegin suiteBegin;
				suiteBegin.name = suite.name;

				parameters.context.Signal(suiteBegin);

				for (const Test& test: suite.testGenerator())
				{
					const TestView view(test);

					event::TestBegin testBegin;
					testBegin.name = view.name;

					parameters.context.Signal(testBegin);

					Requirements requirements(parameters.context.Reporters());
					parameters.context.Run(requirements, view.test);

					event::TestEnd testEnd;
					testEnd.name = view.name;

					parameters.context.Signal(testEnd);
				}

				event::SuiteEnd end;
				end.name = suite.name;

				parameters.context.Signal(end);
			}

			event::Summary summary;
			parameters.context.Signal(summary);

			return {};
		}

		ListResult List(const ListParameters& parameters)
		{
			ListReporterParameters listReporterParameters;
			ListReporter listReporter(listReporterParameters, parameters.logger.CreateLogger("reporter"));

			EmptyRunner runner(log::RootLogger().CreateLogger("empty_runner"));

			Reporter* reporter = &listReporter;
			std::ranges::single_view reporters {reporter};

			Context context(runner, reporters);

			const ExecuteParameters executeParameters(context);
			Execute(executeParameters);

			ListResult result;

			const CumulativeAdapter::CumulativeData& data = listReporter.Data();

			for (auto& suite: data.suites)
			{
				auto& resultSuite = result.suites.emplace_back();

				for (auto& test: suite.tests)
				{
					TestDescription description;
					description.name = test.begin.name;

					resultSuite.tests.push_back(description);
				}
			}

			return result;
		}
	};
}
