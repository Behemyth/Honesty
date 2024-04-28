import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;

namespace
{
	auto suiteGenerator = []()
	{
	};

	Suite suite("suite", suiteGenerator);
	bool result = RegisterSuite(suite);
}
