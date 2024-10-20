
export module synodic.honesty.test.backend:suite_context;

import std;
import synodic.honesty.log;

import :reporter;
import :requirements;

namespace synodic::honesty::test
{
	/**
	 * @brief The context for a suite of tests. All execution within a suite must be synchronous
	 */
	export struct SuiteContext
	{
		explicit SuiteContext(
			const std::span<std::unique_ptr<Reporter>> reporters,
			log::Logger logger) :
			logger(std::move(logger)),
			reporters(reporters)
		{
		}

		Requirements CreateRequirements(const std::string_view testName, const ExpectedTestOutcome outcome) const
		{
			const Requirements::Parameters parameters(testName, outcome);

			return Requirements(reporters, parameters, logger);
		}

		log::Logger logger;

		std::span<std::unique_ptr<Reporter>> reporters;

		const std::span<std::string_view> filter;
	};
}
