import synodic.honesty.test;

import std;

namespace
{
	synodic::honesty::test::CompactReporter reporter("default");
	bool registered = RegisterReporter(reporter);
}
