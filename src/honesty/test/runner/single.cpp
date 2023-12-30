module synodic.honesty.test.runner:single;

import :single;

namespace synodic::honesty::runner
{
	void single_threaded::run() const
	{
		for (const suite_data& suite: suites_)
		{
			for (const TestBase& test: suite.generator_())
			{
				test.Run();
			}
		}
	}

	void single_threaded::submit(suite_data data)
	{
		suites_.push_back(std::move(data));
	}
}
