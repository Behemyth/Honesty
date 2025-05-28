export module synodic.honesty.test.backend:test_context;

import std;
import synodic.honesty.log;
import :reporter;

import :requirements;

namespace honesty::test
{
	/**
	 * @brief A backend for the Requirements class. This is what the testing framework uses to interact with the
	 *	requirements populated by the user. All nested tests must be synchronous.
	 */
	export struct TestContext
	{
		struct OutputData
		{
			explicit OutputData(const Requirements::Output& requirementsOutput) :
				success(requirementsOutput.success)
			{
			}

			bool success;
		};

		explicit TestContext(
			const std::span<Reporter*> reporters,
			log::Logger& logger,
			const std::span<std::string_view> filterViews,
			const bool dryRun) :
			logger(logger),
			reporters(reporters),
			filterViews(filterViews),
			dryRun(dryRun)
		{
		}

		/**
		 * @brief Creates a Requirements object to be passed to an executing test
		 */
		Requirements CreateRequirements(const std::string_view testName, const ExpectedTestOutcome outcome) const
		{
			const Requirements::Parameters parameters(testName, outcome);

			return Requirements(reporters, parameters, logger);
		}

		OutputData Output(const Requirements& requirements) const
		{
			const Requirements::Output& output = requirements.output_;

			return OutputData(output);
		}

		std::reference_wrapper<log::Logger> logger;

		std::span<Reporter*> reporters;

		std::span<std::string_view> filterViews;

		bool dryRun;
	};
}
