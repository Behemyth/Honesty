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
			runner(runner)
		{
		}

		Runner* runner;
	};

	export struct ListParameters
	{
		ListParameters(Runner* runner, Reporter* reporter, log::Logger logger) :
			runner(runner),
			reporter(reporter),
			logger(std::move(logger))
		{
		}

		Runner* runner;
		Reporter* reporter;
		log::Logger logger;
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

				GetThreadContext().Signal(begin);

				for (const Test& test: suite.testGenerator())
				{
					const TestView view(test);

					event::TestBegin testBegin;
					begin.name = view.name;

					GetThreadContext().Signal(testBegin);

					parameters.runner->Run(view.test);

					event::TestEnd testEnd;
					testEnd.name = view.name;

					GetThreadContext().Signal(testEnd);
				}

				event::SuiteEnd end;
				end.name = suite.name;

				GetThreadContext().Signal(end);
			}

			return {};
		}

		ListResult List(const ListParameters& parameters)
		{
			ListResult result;

			const ExecuteParameters executeParameters(parameters.runner, parameters.reporter);

			Execute(executeParameters);

			return result;
		}
	};
}
