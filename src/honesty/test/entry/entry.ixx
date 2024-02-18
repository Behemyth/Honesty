
export module synodic.honesty.test.entry;

import synodic.honesty.test;
import synodic.honesty.test.runner;
import synodic.honesty.test.reporter;
import synodic.honesty.test.backend;

import std;

namespace synodic::honesty
{
}

export namespace synodic::honesty
{
	enum class TestResultCode : std::uint8_t
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
	std::expected<void, TestResultCode>
		entry(const reporter::Console& defaultReporter, Runner& defaultRunner)
	{
		const auto suites = Registry::GetDefaultSuites();

		const std::span<const Reporter* const> reporters = Registry::GetReporters();
		const auto runners = Registry::GetRunners();

		Events events(reporters);

		// Move the direct registry data into the default defaultRunner
		defaultRunner.Submit(suites);
		defaultRunner.Run(events);

		return {};
	}
}
