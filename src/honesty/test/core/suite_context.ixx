
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
	export struct SuiteContext
	{
		explicit SuiteContext(Runner& runner, log::Logger logger) :
			logger(std::move(logger)),
			runner(runner)
		{
		}

		TestContext CreateTestContext(const std::span<std::unique_ptr<Reporter>> reporters) const
		{
			return TestContext(reporters, logger);
		}

		log::Logger logger;

		std::reference_wrapper<Runner> runner;
		std::span<std::unique_ptr<Reporter>> reporters;

		const std::span<std::string_view> filter;
	};
}
