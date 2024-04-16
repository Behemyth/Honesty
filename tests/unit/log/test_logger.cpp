import std;
import synodic.honesty.test;
import synodic.honesty.log;

using namespace synodic::honesty::test::literals;

namespace
{
	synodic::honesty::test::Suite loggerSuite(
		"logger",
		[]() -> synodic::honesty::test::generator<synodic::honesty::test::TestBase>
		{
			co_yield "TODO"_test = []()
			{
			};
		});

	bool registered = Register(loggerSuite);
}
