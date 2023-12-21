import std;
import synodic.honesty.test;

using namespace synodic::honesty;

namespace
{
	auto suiteGenerator = []() -> TestGenerator
	{
		co_return;
	};

	// Verify that the suite is compiling
	suite suite("suite suite", suiteGenerator);
}
