module synodic.honesty.test;

import :runner.default;

namespace synodic::honesty::test
{
	void DefaultRunner::Run(std::function_ref<void()> function)
	{
		function();
	}
}
