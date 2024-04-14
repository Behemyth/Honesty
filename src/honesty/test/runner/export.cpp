import synodic.honesty.test;
import synodic.honesty.test.runner.local;

namespace
{
	constinit synodic::honesty::test::Local runner("default");
	bool registered = RegisterRunner(runner);
}
