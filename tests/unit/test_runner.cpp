import std;
import synodic.honesty.test;
import synodic.honesty.test.runner;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto suiteGenerator = []() -> Generator
	{
		co_return;
	};

	suite suite("runner suite", suiteGenerator);
}

static_assert(is_runner<runner::single_threaded>, "The single threaded runner must satisfy the runner concept");
