import synodic.honesty.test;
import synodic.honesty.test.runner.local;

namespace
{
	constinit synodic::honesty::test::runner::Local runner("default");
	bool registered = RegisterRunner(runner);
}
