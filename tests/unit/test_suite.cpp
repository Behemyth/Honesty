import std;
import synodic.honesty.test;
import synodic.honesty.test.backend;
import generator;
using namespace synodic::honesty;

namespace
{
	auto suiteGenerator = []() -> TestGenerator
	{
		co_yield Test(
			"top level test",
			[]()
			{
			});
	};

	// Registration
	suite suite("suite suite", suiteGenerator);
}
