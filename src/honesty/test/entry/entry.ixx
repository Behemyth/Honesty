
export module synodic.honesty.test:entry;

import std;
import :backend;
import :reporter;
import :reporter.console;
import :runner;
import :runner.local;

namespace synodic::honesty
{
}

namespace synodic::honesty::test
{
	export enum class TestResultCode : std::uint8_t
	{
		FAILURE
	};

	/**
	 * @brief The entry point for the test runner. If the user does not specify a runner, the default runner will be
	 *	used to run the tests
	 * @param reporter
	 * @param defaultRunner The default runner to use when running tests
	 * @return The result of the test run
	 */
	export template<typename ReporterT = reporter::Console, typename RunnerT = runner::Local>
	std::expected<void, TestResultCode> entry(ReporterT& defaultReporter, RunnerT& defaultRunner)
	{
		Registry registry(defaultReporter, defaultRunner);

		std::span<Reporter*> reporters = registry.GetReporters();
		std::span<Runner*> runners	   = registry.GetRunners();

		Broadcast broadcaster;

		for (Runner* runner: runners)
		{
			runner->Run(broadcaster);
		}

		return {};
	}
}
