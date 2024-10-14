
export module synodic.honesty.test.backend:suite_context;

import std;
import synodic.honesty.log;
import synodic.honesty.test.types;
import :test_context;

namespace synodic::honesty::test
{
	/**
	 * @brief The context for a suite of tests. All execution within a suite must be synchronous
	 */
	export struct SuiteContext : Fixture
	{
		// NOTE: We inherit from Fixture to pull data that the user creates without exposing it publicly

		explicit SuiteContext(Runner& runner, log::Logger logger) :
			logger(std::move(logger)),
			runner(runner),
			Fixture(applicationName, suiteName, logger)
		{
		}

		TestContext CreateTestContext(
			const std::span<std::unique_ptr<Reporter>> reporters,
			const RequirementParameters& input) const
		{
			return TestContext(reporters, input, logger);
		}

		log::Logger logger;

		std::reference_wrapper<Runner> runner;
		std::span<std::unique_ptr<Reporter>> reporters;

		const std::span<std::string_view> filter;
	};
}
