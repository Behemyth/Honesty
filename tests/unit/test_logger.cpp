import std;
import synodic.honesty.test;
import synodic.honesty.test.logger;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto suiteGenerator = []() -> Generator
	{
		log("message");

		co_return;
	};

	constexpr suite suite("logger suite", suiteGenerator);
}

static_assert(is_logger<logger::StandardOut>, "The stdout logger must satisfy the logger concept");
