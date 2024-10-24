export module synodic.honesty.test.backend:test_context;

import std;
import synodic.honesty.log;
import :reporter;

import :requirements;

namespace synodic::honesty::test
{
	/**
	 * @brief A backend for the Requirements class. This is what the testing framework uses to interact with the
	 *	requirements populated by the user. All nested tests must be synchronous.
	 */
	export struct TestContext
	{
		explicit TestContext(const log::Logger& logger)
		{
		}

		/**
		 * @brief Creates a Requirements object to be passed to a executing test
		 */
		auto CreateRequirements(const std::string_view testName, const ExpectedTestOutcome testOutcome) const
			-> Requirements
		{
			return Requirements();
		}


		std::span<std::string_view> filter;
	};
}
