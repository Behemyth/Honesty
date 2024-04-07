module synodic.honesty.test:runner.local;

import std;
import :runner.local;

namespace synodic::honesty::test::runner
{
	void Local::Run(Broadcast& broadcaster)
	{
		for (const SuiteData* const suite: suites_)
		{
			event::SuiteBegin event(suite->Name());

			broadcaster.signal(event);

			auto generator = suite->Generator();
			for (const TestBase& test: generator())
			{
				event::TestBegin beginEvent(test.Name());

				broadcaster.signal(beginEvent);

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
