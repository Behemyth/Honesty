import synodic.honesty.test;

namespace
{
	constinit synodic::honesty::test::Local runner("default");
	bool registered = RegisterRunner(runner);
}
