module synodic.honesty.test.runner:local;

import :local;

namespace synodic::honesty::runner
{
	void Local::Run(const Events& events)
	{
		for (const SuiteData* const suite: suites_)
		{
			synodic::honesty::event::SuiteBegin event(suite->name);

			events.signal(event);

			for (const TestBase& test: suite->generatorWrapper())
			{
				synodic::honesty::event::TestBegin event(test.Name());

				events.signal(event);

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
