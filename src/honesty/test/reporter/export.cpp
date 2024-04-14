import synodic.honesty.test;
import synodic.honesty.test.reporter.console;
import synodic.honesty.log;

namespace
{
	constexpr synodic::honesty::log::Logger logger("reporter");
	constinit synodic::honesty::test::Console reporter("default", logger);
	bool registered = RegisterReporter(reporter);
}
