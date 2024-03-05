
module synodic.honesty.test:runner;

import :backend;
import :runner;

/**
 * @brief The type is already constrained via the reporter interface. No need to pollute the definition here with
 * duplicate constraints.
 * @param event
 */
template<typename Event>
void delegate(const Event& event)
{
	for (synodic::honesty::test::Reporter* reporter: synodic::honesty::test::Registry::GetReporters())
	{
		reporter->signal(event);
	}
}

namespace synodic::honesty::test
{
	void Broadcast::signal(const event::SuiteBegin& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::SuiteEnd& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::SuiteSkip& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::SuiteRun& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::SuiteFail& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::SuitePass& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::SuiteSummary& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::TestBegin& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::TestEnd& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::TestSkip& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::TestRun& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::TestFail& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::TestPass& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::AssertionFail& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::AssertionPass& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::AssertionSkip& event)
	{
		delegate(event);
	}

	void Broadcast::signal(const event::Summary& event)
	{
		delegate(event);
	}
}
