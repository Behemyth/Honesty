import std;
import synodic.honesty.test;
import synodic.honesty.test.backend;
import generator;
using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto suiteGenerator = []() -> std::generator<TestBase>
	{
		co_yield "exception"_test = []
		{
			// expect_throw<std::runtime_error>(true);
			// expect_throw();
		};
	};

	suite suite("exception suite", suiteGenerator);
}
