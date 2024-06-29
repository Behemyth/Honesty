import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"fixture",
		[]() -> Generator
		{
			co_yield "log"_test = [](const Requirements& requirements)
			{
			};
		});
	SuiteRegistrar _(SUITE);
}
