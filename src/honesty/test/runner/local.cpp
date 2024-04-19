module synodic.honesty.test;

namespace synodic::honesty::test
{
	void Local::Run(std::span<Set> sets, std::function_ref<RunnerContext&()> generateContext)
	{
		const auto& context = generateContext();

		for (const Set& set: sets)
		{
			event::SuiteBegin event(set.Name());

			context.broadcaster.Signal(event);

			for (const TestBase& test: set.YieldTests())
			{
				event::TestBegin beginEvent(test.Name());

				context.broadcaster.Signal(beginEvent);

				test.Run();
			}
		}
	}
}
