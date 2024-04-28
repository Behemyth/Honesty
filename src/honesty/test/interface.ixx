export module synodic.honesty.test:interface;

import synodic.honesty.log;
import std;
import :registry;
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
		ExecuteParameters(Runner* runner, Reporter* reporter);

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
		ListParameters();

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

		explicit Interface(const Configuration& configuration);

		HelpResult Help(const HelpParameters& parameters);
		ExecuteResult Execute(const ExecuteParameters& parameters);
		ListResult List(const ListParameters& parameters);

	private:
	};
}

module synodic.honesty.test:interface;

import std;
import :registry;
import :reporter;
import :runner;
import :test;
import :context;

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

		Context& context = GetContext();
		std::ranges::single_view reporters {parameters.reporter};

		// Before starting a suite, we need to set up the current thread's context
		context = Context(parameters.runner, reporters);

		for (SuiteData* suite: suites)
		{
			parameters.runner->Run(suite->generator);
		}

		return {};
	}

	ListResult Interface::List(const ListParameters& parameters)
	{
		auto suites = GetRegistry().GetSuites();

		ListResult result;

		// for (const SuiteData* const suite: suites)
		//{
		//	auto generator = suite->Generator();
		//	for (const TestBase& test: generator())
		//	{
		//		TestDescription description;
		//		description.name = test.Name();

		//		result.tests.push_back(description);
		//	}
		//}

		return result;
	}

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

	void RegisterSuite(SuiteData& suite)
	{
		GetRegistry().AddSuite(suite);
	}

}
