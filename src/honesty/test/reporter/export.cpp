import synodic.honesty.test;
import synodic.honesty.test.reporter.console;
import synodic.honesty.test.logger.console;

namespace
{
	constexpr synodic::honesty::test::logger::Console logger;
	constinit synodic::honesty::test::reporter::Console reporter("default", logger);
	bool registered = RegisterReporter(reporter);
}
