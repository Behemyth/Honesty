module synodic.honesty.test.runner:local;

import :local;

namespace synodic::honesty::runner
{
	void Local::Run() const
	{
		for (const SuiteData* const suite: suites_)
		{
			for (const TestBase& test: suite->generator_())
			{
				test.Run();
			}
		}
	}

	void Local::Submit(const SuiteData* data)
	{
		suites_.push_back(std::move(data));
	}

	void Local::Submit(std::span<const SuiteData* const> data)
	{
		suites_.insert(suites_.end(), std::make_move_iterator(data.begin()), std::make_move_iterator(data.end()));
	}
}
