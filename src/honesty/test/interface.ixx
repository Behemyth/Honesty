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
		ExecuteParameters(Runner* runner, Reporter* reporter) :
			runner(runner),
			reporter(reporter)
		{
		}

		Runner* runner;
		Reporter* reporter;
	};

	export struct ListParameters
	{
		ListParameters(Runner* runner, log::Logger logger) :
			runner(runner),
			logger(std::move(logger)),
			outputType(ListOutputType::LOG)
		{
		}

		Runner* runner;
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

	export struct ListResult
	{
		ListResult() = default;

		std::vector<TestDescription> tests;
	};

	/**
	 * @brief The interface to interacting with the test framework. Multiple interfaces are allowed to exist for the
	 *	same instance
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
			// If we modify the execution context we should do it outside this function

			for (const SuiteView& suite: GetSuites())
			{
				event::SuiteBegin begin;
				begin.name = suite.name;

				GetContext().Signal(begin);

				for (const Test& test: suite.testGenerator())
				{
					const TestView view(test);

					event::TestBegin testBegin;
					begin.name = view.name;

					GetContext().Signal(testBegin);

					parameters.runner->Run(view.test);

					event::TestEnd testEnd;
					testEnd.name = view.name;

					GetContext().Signal(testEnd);
				}

				event::SuiteEnd end;
				end.name = suite.name;

				GetContext().Signal(end);
			}

			return {};
		}

		ListResult List(ListParameters& parameters)
		{
			ListResult result;

			Execute(executeParameters);

			return result;
		}
	};
}
