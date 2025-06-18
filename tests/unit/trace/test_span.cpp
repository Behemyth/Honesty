import std;
import synodic.honesty.test;
import synodic.honesty.trace;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	Suite SUITE(
		"span",
		[]() -> Generator
		{
			co_yield TODO / "span"_test = [](const Requirements& requirements)
			{
					honesty::Span span("span");
			};
		});
	SuiteRegistrar _(SUITE);
}
