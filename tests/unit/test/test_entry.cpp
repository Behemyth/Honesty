import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;


namespace
{
	auto entryGenerator = []() -> generator<TestBase>
	{
		co_return;
	};

	Suite suite("suite", entryGenerator);
	bool result = suite.Register();
}
