module synodic.honesty.test;
import :types;
import :context;

namespace synodic::honesty::test
{
	void Signal(const event::AssertionPass& passed)
	{
		GetContext().Signal(passed);
	}

	void Signal(const event::AssertionFail& failed)
	{
		GetContext().Signal(failed);
	}

	void Assert(bool expression, const std::source_location& location)
	{
		if (expression)
		{
			event::AssertionPass passed;
			passed.location = location;

			Signal(passed);
		}
		else
		{
			event::AssertionFail failed;
			failed.location = location;

			Signal(failed);
			throw AssertException("Assertion failed");
		}
	}

	void Expect(bool expression, const std::source_location& location)
	{
		if (expression)
		{
			event::AssertionPass passed;
			passed.location = location;

			Signal(passed);
		}
		else
		{
			event::AssertionFail failed;
			failed.location = location;

			Signal(failed);
		}
	}
}
