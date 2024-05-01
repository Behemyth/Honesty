export module synodic.honesty.test:interface;

import synodic.honesty.log;
import std;
import :suite;
import :registry;
import :context;
import :reporter;
import :runner;
import :test;

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

	export enum class ListOutputType : std::uint8_t
	{
		LOG,
		JSON
	};

	export struct ListParameters
	{
		ListParameters(Runner* runner) :
			runner(runner),
			outputType(ListOutputType::LOG)
		{
		}

		Runner* runner;
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
			const auto suites = GetRegistry().GetSuites();

			//Context& context = GetContext();
			//std::ranges::single_view reporters {parameters.reporter};

			//// Before starting a suite, we need to set up the current thread's context
			//context = Context(*parameters.runner, reporters);

			for (const SuiteView& suite: suites)
			{
				auto wrapper = [this, suite]
				{
					SuiteWrapper(suite);
				};

				parameters.runner->Run(wrapper);
			}

			return {};
		}

		ListResult List(const ListParameters& parameters)
		{
			const auto suites = GetRegistry().GetSuites();

			//Context& context = GetContext();

			//std::ranges::single_view reporters {&GetListReporter()};
			//context = Context(*parameters.runner, reporters);

			ListResult result;

			for (const SuiteView& suite: suites)
			{
				auto wrapper = [this, suite]
				{
					SuiteWrapper(suite);
				};

				parameters.runner->Run(wrapper);
			}

			return result;
		}

	private:
		void SuiteWrapper(const SuiteView& suite)
		{
			event::SuiteBegin begin;
			begin.name = suite.name;

			GetContext().Signal(begin);

			//suite.tests();

			event::SuiteEnd end;
			end.name = suite.name;

			GetContext().Signal(end);
		}
	};

	/**
	 * @brief Registers a runner object with the test framework
	 * @param runner The runner object to register
	 * @return Whether the runner was successfully registered
	 */
	export bool RegisterRunner(Runner& runner)
	{
		GetRegistry().AddRunner(runner);
		return true;
	}

	/***
	 * @brief Registers a reporter with the test framework
	 * @param reporter The reporter to register
	 * @return Whether the reporter was successfully registered
	 */
	export bool RegisterReporter(Reporter& reporter)
	{
		GetRegistry().AddReporter(reporter);
		return true;
	}

	export template<size_t... NameSizes>
	bool RegisterSuite(Suite<NameSizes>&... suites)
	{
		(GetRegistry().AddSuite(suites), ...);

		return true;
	}
}
