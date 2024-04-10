module synodic.honesty.test:runner;

import :types;
import :reporter;
import :registry;
import std;

namespace synodic::honesty::test
{
	Broadcast::Broadcast(std::span<Reporter*> reporters) :
		reporters_(reporters)
	{
	}

	void Broadcast::signal(const event::SuiteBegin& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::SuiteEnd& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::SuiteSkip& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::SuiteRun& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::SuiteFail& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::SuitePass& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::SuiteSummary& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::TestBegin& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::TestEnd& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::TestSkip& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::TestRun& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::TestFail& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::TestPass& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::AssertionFail& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::AssertionPass& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::AssertionSkip& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::Summary& event)
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	RunnerContext::RunnerContext(std::span<Reporter*> reporters) :
		broadcaster(reporters)
	{
	}
}
