module synodic.honesty.test;

namespace synodic::honesty::test
{
	void assert(bool expression, const std::source_location& location)
	{
		if (expression)
		{
			event::AssertionPass passed;
			passed.location = location;

			GetContext().broadcaster.signal(passed);
		}
		else
		{
			event::AssertionFail failed;
			failed.location = location;

			GetContext().broadcaster.signal(failed);

			// TODO: Quit the specific test
		}
	}

	bool expect(bool expression, const std::source_location& location)
	{
		if (expression)
		{
			event::AssertionPass passed;
			passed.location = location;

			GetContext().broadcaster.signal(passed);
		}
		else
		{
			event::AssertionFail failed;
			failed.location = location;

			GetContext().broadcaster.signal(failed);
		}

		return expression;
	}
}
