
export module synodic.honesty.test.backend:suite_context;

import std;
import synodic.honesty.log;

import :reporter;
import :fixture;

namespace honesty::test
{
	/**
	 * @brief The context for a suite of tests. All execution within a suite must be synchronous
	 */
	export struct SuiteContext
	{
		explicit SuiteContext(
			const std::span<std::unique_ptr<Reporter>> reporters,
			log::Logger& logger,
			const std::string_view applicationName,
			const std::string_view suiteName,
			const std::span<std::string_view> filterViews,
			const bool dryRun) :
			logger(logger),
			reporters(reporters),
			applicationName(applicationName),
			suiteName(suiteName),
			filterViews(filterViews),
			dryRun(dryRun)
		{
		}

		/**
		 * @brief Creates a Fixture object to be passed to an executing suite
		 */
		Fixture CreateFixture()
		{
			return Fixture(reporters, applicationName, suiteName, logger);
		}

		std::reference_wrapper<log::Logger> logger;

		std::span<std::unique_ptr<Reporter>> reporters;

		std::string_view applicationName;
		std::string_view suiteName;

		std::span<std::string_view> filterViews;

		bool dryRun;
	};
}
