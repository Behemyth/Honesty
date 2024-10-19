export module synodic.honesty.test.backend:test_context;

import std;
import synodic.honesty.log;
import :reporter;

namespace synodic::honesty::test
{
	/**
	 * @brief A backend for the Requirements class. This is what the testing framework uses to interact with the
	 *	requirements populated by the user. All nested tests must be synchronous.
	 */
	export struct TestContext
	{
		TestContext(const std::span<std::unique_ptr<Reporter>> reporters, const log::Logger& logger)
		{
		}
	};
}
