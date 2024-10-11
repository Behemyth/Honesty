import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"test_log",
		[](const Fixture& fixture) -> Generator
		{
			//fixture.

			// Verify that you can capture the information going into the root logger
			co_yield "test"_test = [](const Requirements& requirements)
			{
			};
		});
	SuiteRegistrar _(SUITE);
}
