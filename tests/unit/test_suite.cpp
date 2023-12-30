import std;
import synodic.honesty.test;

using namespace synodic::honesty;

namespace
{
	auto suiteGenerator = []() -> TestGenerator
	{
		co_return;
	};

	// Verify that the Suite is compiling
	Suite suite("Suite Suite", suiteGenerator);
}
