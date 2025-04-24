import std;
import synodic.honesty.test;
import synodic.honesty.trace;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	Suite SUITE(
		"provider",
		[]() -> Generator
		{
			co_yield "empty_construction"_test = [](const Requirements& requirements)
			{
				honesty::trace::Provider provider("name");
			};

		});
	SuiteRegistrar _(SUITE);
}
