module synodic.honesty.test.runner:local;

import :local;

namespace synodic::honesty::runner
{
	void Local::Run()
	{
		for (const SuiteData& suite: suites_)
		{
			for (const TestBase& test: suite.generator_())
			{
				test.Run();
			}
		}
	}

	void Local::Submit(SuiteData data)
	{
		suites_.push_back(std::move(data));
	}

	void Local::Submit(std::vector<SuiteData> data)
	{
		suites_.insert(suites_.end(), std::make_move_iterator(data.begin()), std::make_move_iterator(data.end()));
	}
}
