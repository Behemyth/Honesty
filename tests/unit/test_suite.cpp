import std;
import synodic.honesty.test;
import synodic.honesty.test.backend;
import generator;
using namespace synodic::honesty;

namespace
{
	auto suiteGenerator = []() -> std::generator<TestBase>
	{
		co_yield Test(
			"top level test",
			[]() -> std::generator<TestBase>
			{
				co_return;
			});
	};

	// Registration
	suite suite("suite suite", suiteGenerator);
}
