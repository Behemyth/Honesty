import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"test_log",
		[]() -> Generator
		{
			// Verify that you can capture the information going into the root logger

			// TODO: Do we support error descriptions on tests?
			co_yield "test"_test = [](const Requirements& requirements)
			{
			};
		});
	SuiteRegistrar _(SUITE);
}
