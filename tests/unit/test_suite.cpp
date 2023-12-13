import std;
import synodic.honesty.test;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto suiteGenerator = []() -> Generator
	{
		co_yield Test(
			"outer",
			[]() -> Generator
			{
				co_return;
			});
	};

	// Global creation
	suite suite("outer", suiteGenerator);
}
