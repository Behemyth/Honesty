import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;

namespace
{
	auto suiteGenerator = []() -> generator<TestBase>
	{
		co_return;
	};

	Suite suite("suite", suiteGenerator);

}
