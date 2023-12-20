import std;
import synodic.honesty.test;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto suiteGenerator = []() -> Generator
	{
		co_yield "exception"_test = []
		{
			// expect_throw<std::runtime_error>(true);
			// expect_throw();
		};
	};

	constexpr suite suite("exception suite", suiteGenerator);
}
