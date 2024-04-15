import synodic.honesty.test;
import synodic.honesty.test.reporter.console;

import std;

namespace
{
	synodic::honesty::test::Console reporter("default");
	bool registered = RegisterReporter(reporter);
}
