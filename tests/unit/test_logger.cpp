import std;
import synodic.honesty.test;
import synodic.honesty.test.logger;
import synodic.honesty.test.backend;
import generator;
using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto suiteGenerator = []() -> TestGenerator
	{
		log("message");

		co_return;
	};

	suite suite("logger suite", suiteGenerator);
}

static_assert(is_logger<logger::StandardOut>, "The stdout logger must satisfy the logger concept");
