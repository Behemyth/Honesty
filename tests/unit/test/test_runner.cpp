import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	auto runnerGenerator = []() -> generator<TestBase>
	{
		co_yield "declare default"_test = []()
		{
		};
	};
	Suite suite("runner", runnerGenerator);
	bool result = suite.Register();
}
