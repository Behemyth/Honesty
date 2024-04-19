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

	void Broadcast::Signal(const event::SuiteBegin& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::SuiteEnd& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::SuiteSkip& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::SuiteRun& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::SuiteFail& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::SuitePass& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::SuiteSummary& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::TestBegin& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::TestEnd& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::TestSkip& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::TestRun& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::TestFail& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::TestPass& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::AssertionFail& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::AssertionPass& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::AssertionSkip& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Broadcast::Signal(const event::Summary& event) const
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

	Set::Set(const SuiteData& data) :
		name_(data.Name())
	{
	}

	std::string_view Set::Name() const
	{
		return name_;
	}
}
