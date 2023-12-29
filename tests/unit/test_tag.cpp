import std;
import synodic.honesty.test;
import synodic.honesty.test.backend;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto dummyGenerator = []() -> TestGenerator
	{
		co_return;
	};

	auto suiteGenerator = []() -> TestGenerator
	{
		co_yield Tag("test") / "inner"_test = dummyGenerator;
		co_yield skip / "test"_tag / "inner"_test = dummyGenerator;
	};

	suite suite("tag suite", suiteGenerator);
}
