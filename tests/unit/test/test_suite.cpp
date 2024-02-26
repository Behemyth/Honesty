import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;

namespace
{
	auto suiteGenerator = []() -> generator<TestBase>
	{
		co_return;
	};

	// Verify that the Suite is compiling. 'constinit' is not required because Suite is an immediate function
	/* constinit */ Suite suite("Suite Suite", suiteGenerator);
	bool registered = suite.Register();

}
