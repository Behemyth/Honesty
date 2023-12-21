import std;
import synodic.honesty.test;
import synodic.honesty.test.runner;
import synodic.honesty.test.backend;
import generator;
using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto suiteGenerator = []() -> TestGenerator
	{
		co_return;
	};

	suite suite("runner suite", suiteGenerator);
}

static_assert(is_runner<runner::single_threaded>, "The single threaded runner must satisfy the runner concept");
