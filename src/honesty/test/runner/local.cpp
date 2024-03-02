module synodic.honesty.test:runner.local;

import :runner.local;

namespace synodic::honesty::test::runner
{
	void Local::Run(const Events& events)
	{
		for (const SuiteData* const suite: suites_)
		{
			synodic::honesty::test::event::SuiteBegin event(suite->Name());

			events.signal(event);


			auto generator = suite->Generator();
			for (const TestBase& test: generator())
			{
				synodic::honesty::test::event::TestBegin event(test.Name());

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
