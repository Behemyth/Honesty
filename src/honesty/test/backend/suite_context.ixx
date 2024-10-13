
export module synodic.honesty.test.backend:suite_context;

import std;
import synodic.honesty.log;
import synodic.honesty.test.types;

namespace synodic::honesty::test
{
	/**
	 * @brief The context for a suite of tests. All execution within a suite must be synchronous
	 */
	export class SuiteContext
	{
	public:
		explicit SuiteContext(log::Logger logger) :
			logger_(std::move(logger))
		{
		}

		Fixture CreateFixture(const std::string_view applicationName, const std::string_view suiteName)
		{
			return Fixture(applicationName, suiteName, logger_);
		}

		TestContext CreateRequirements(
			const std::span<std::unique_ptr<Reporter>> reporters,
			const RequirementParameters& input) const
		{
			return TestContext(reporters, input, logger_);
		}

		const log::Logger& Logger() const
		{
			return logger_;
		}

		std::span<std::unique_ptr<Reporter>> Reporters() const
		{
			return reporters_;
		}

	private:
		log::Logger logger_;

		std::span<std::unique_ptr<Reporter>> reporters_;
	};
}
