module synodic.honesty.test:runner;

import :types;
import :reporter;
import :registry;
import std;

namespace synodic::honesty::test
{
	Broadcast::Broadcast(std::span<Reporter*> reporters):
		reporters_(reporters)
	{
	}

	void Broadcast::signal(const event::SuiteBegin& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::SuiteEnd& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::SuiteSkip& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::SuiteRun& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::SuiteFail& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::SuitePass& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::SuiteSummary& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::TestBegin& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::TestEnd& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::TestSkip& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::TestRun& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::TestFail& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::TestPass& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::AssertionFail& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::AssertionPass& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::AssertionSkip& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::signal(const event::Summary& event) const
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
