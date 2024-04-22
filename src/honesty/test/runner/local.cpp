module synodic.honesty.test;

namespace synodic::honesty::test
{
	void Local::Run(std::function_ref<void()> function)
	{
		function();
	}
}
