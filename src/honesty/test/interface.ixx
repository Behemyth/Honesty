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

namespace
{
	// A helper for variant overload deduction
	template<typename... Ts>
	struct Overload : Ts...
	{
		using Ts::operator()...;
	};
}

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
		explicit ExecuteParameters(const Context& context) :
			context(context)
		{
		}

		Context context;
	};

	export struct ListParameters
	{
		explicit ListParameters(log::Logger logger) :
			logger(std::move(logger)),
			outputType(ListOutputType::LOG)
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
		explicit ExecuteResult(const bool success) :
			success(success)
		{
		}

		bool success;
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
			explicit Configuration(std::string applicationName) :
				applicationName(std::move(applicationName))
			{
			}

			std::string applicationName;
		};

		explicit Interface(const Configuration& configuration) :
			applicationName_(configuration.applicationName)
		{
		}

		HelpResult Help(const HelpParameters& parameters)
		{
			return {};
		}

		[[nodiscard]] ExecuteResult Execute(const ExecuteParameters& parameters)
		{
			for (const SuiteView& suite: GetSuites())
			{
				event::SuiteBegin suiteBegin;
				suiteBegin.name = suite.name;

				parameters.context.Signal(suiteBegin);

				// Fixture lifetime should be for the whole suite	
				Fixture fixture(applicationName_, suite.name);

				auto executor = Overload {
					[&](const std::function_ref<Generator()> generator) -> Generator
					{
						return generator();
					},
					[&](const std::function_ref<Generator(Fixture&)> generator) -> Generator
					{
						return generator(fixture);
					}};

				Generator generator = std::visit(executor, suite.testGenerator);

				for (const Test& test: generator)
				{
					const TestView view(test);

					event::TestBegin testBegin;
					testBegin.name = view.name;

					parameters.context.Signal(testBegin);

					Requirements requirements(parameters.context.Reporters(), test.Name());
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

			bool success = true;

			// TODO: Extract test state

			return ExecuteResult(success);
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
			ExecuteResult executeResult = Execute(executeParameters);

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

	private:
		std::string applicationName_;
	};
}
