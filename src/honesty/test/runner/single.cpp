module synodic.honesty.test.runner:single;

import :single;

namespace synodic::honesty::runner
{
	void single_threaded::Run()
	{
		for (const suite_data& suite: suites_)
		{
			for (const TestBase& test: suite.generator_())
			{
				test.Run();
			}
		}
	}

	void single_threaded::Submit(suite_data data)
	{
		suites_.push_back(std::move(data));
	}

	void single_threaded::Submit(std::vector<suite_data> data)
	{
		suites_.insert(suites_.end(), std::make_move_iterator(data.begin()), std::make_move_iterator(data.end()));
	}
}
