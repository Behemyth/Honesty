module synodic.honesty.test;
import :types;

namespace synodic::honesty::test
{
	void assert(bool expression, const std::source_location& location)
	{
		if (expression)
		{
			event::AssertionPass passed;
			passed.location = location;

			GetContext().broadcaster.Signal(passed);
		}
		else
		{
			event::AssertionFail failed;
			failed.location = location;

			GetContext().broadcaster.Signal(failed);

			// TODO: Quit the specific test
			throw Assert("Assertion failed");
		}
	}

	bool expect(bool expression, const std::source_location& location)
	{
		if (expression)
		{
			event::AssertionPass passed;
			passed.location = location;

			GetContext().broadcaster.Signal(passed);
		}
		else
		{
			event::AssertionFail failed;
			failed.location = location;

			GetContext().broadcaster.Signal(failed);
		}

		return expression;
	}
}
