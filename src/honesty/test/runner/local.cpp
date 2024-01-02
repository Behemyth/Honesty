module synodic.honesty.test.runner:local;

import :local;

namespace synodic::honesty::runner
{
	void Local::Run()
	{
		for (const suite_data& suite: suites_)
		{
			for (const TestBase& test: suite.generator_())
			{
				test.Run();
			}
		}
	}

	void Local::Submit(suite_data data)
	{
		suites_.push_back(std::move(data));
	}

	void Local::Submit(std::vector<suite_data> data)
	{
		suites_.insert(suites_.end(), std::make_move_iterator(data.begin()), std::make_move_iterator(data.end()));
	}
}
