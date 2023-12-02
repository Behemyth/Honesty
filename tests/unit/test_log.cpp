import std;
import synodic.honesty.test;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto suiteGenerator = []() -> Generator
	{
		log("message");

		co_return;
	};

	Suite suite("outer", suiteGenerator);
}
