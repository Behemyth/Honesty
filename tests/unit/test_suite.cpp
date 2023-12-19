import std;
import synodic.honesty.test;

using namespace synodic::honesty;

namespace
{
	auto suiteGenerator = []() -> Generator
	{
		co_yield Test(
			"top level test",
			[]() -> Generator
			{
				co_return;
			});
	};

	// Registration
	constexpr suite suite("suite suite", suiteGenerator);
}
