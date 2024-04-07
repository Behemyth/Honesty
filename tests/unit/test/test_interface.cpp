import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;


namespace
{
	auto interfaceGenerator = []() -> generator<TestBase>
	{
		co_return;
	};

	Suite suite("suite", interfaceGenerator);
	bool result = suite.Register();
}
