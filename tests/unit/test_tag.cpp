import std;
import synodic.honesty.test;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto dummyGenerator = []() -> Generator
	{
		co_return;
	};

	auto suiteGenerator = []() -> Generator
	{
		//co_yield tag("test") / "inner"_test = dummyGenerator;
		//co_yield skip / "test"_tag / "inner"_test = dummyGenerator;
		co_return;
	};

	constexpr suite suite("tag suite", suiteGenerator);
}
