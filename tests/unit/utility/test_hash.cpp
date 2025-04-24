import std;
import synodic.honesty.test;
import synodic.honesty.utility;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	Suite SUITE(
		"hash",
		[]() -> Generator
		{
			co_yield "hash"_test = [](const Requirements&)
			{
			};
		});

	SuiteRegistrar _(SUITE);
}
