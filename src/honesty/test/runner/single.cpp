module synodic.honesty.test.runner:single;

import :single;

namespace synodic::honesty::runner
{
	void single_threaded::run(std::span<std::move_only_function<Generator()>> suites) const
	{
		for (auto& suite: suites)
		{
			for (auto generator = suite(); const Test& test: generator)
			{
				//test.run();
			}
		}
	}
}
