export module synodic.honesty.test.backend:test_context;

import std;
import synodic.honesty.log;
import synodic.honesty.test.types;

namespace synodic::honesty::test
{
	/**
	 * @brief A backend for the Requirements class. This is what the testing framework uses to interact with the
	 *	requirements populated by the user. All nested tests must be synchronous.
	 */
	export struct TestContext
	{
		TestContext(
			const std::span<std::unique_ptr<Reporter>> reporters,
			const RequirementParameters& input,
			const log::Logger& logger) :
			Requirements(reporters, input, logger)
		{
		}

		const RequirementParameters& Parameters() const
		{
			return parameters_;
		}

		/**
		 * @brief Returns the part of the base class that can't be modified or seen directly by the user
		 * @return The mutable context of the constant requirements
		 */
		const RequirementOutput& Output() const
		{
			return output_;
		}
	};
}
