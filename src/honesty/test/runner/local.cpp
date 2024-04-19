module synodic.honesty.test;

namespace synodic::honesty::test
{
	void Local::Run(std::function_ref<void(RunnerContext& context)> generateContext)
	{
		RunnerContext& context = GetContext();

		generateContext(context);
	}
}
