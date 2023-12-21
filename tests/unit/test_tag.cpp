import std;
import synodic.honesty.test;
import synodic.honesty.test.backend;
import generator;
using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto dummyGenerator = []() -> std::generator<TestBase>
	{
		co_return;
	};

	auto suiteGenerator = []() -> std::generator<TestBase>
	{
		//co_yield tag("test") / "inner"_test = dummyGenerator;
		//co_yield skip / "test"_tag / "inner"_test = dummyGenerator;
		co_return;
	};

	suite suite("tag suite", suiteGenerator);
}
