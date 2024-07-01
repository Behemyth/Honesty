import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"runner",
		[]() -> Generator
		{
			co_yield "declare_default"_test = [](const Requirements& requirements)
			{
			};
		});
	SuiteRegistrar _(SUITE);
}
