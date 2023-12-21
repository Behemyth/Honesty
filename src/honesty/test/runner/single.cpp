module synodic.honesty.test.runner:single;

import :single;

namespace synodic::honesty::runner
{
	void single_threaded::run(std::span<const suite_data> suites) const
	{
		for (const suite_data& suite: suites)
		{
			for (const TestBase& test: suite.generator_())
			{
				test.Run();
			}
		}
	}
}
