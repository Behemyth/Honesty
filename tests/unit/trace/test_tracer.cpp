import std;
import synodic.honesty.test;
import synodic.honesty.trace;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	Suite SUITE(
		"tracer",
		[]() -> Generator
		{
			co_yield "construction"_test = [](const Requirements& requirements)
			{
				constexpr honesty::trace::Provider provider("one", "two", "three");

				constexpr honesty::trace::Tracer one = provider.Get("one");
			};
		});
	SuiteRegistrar _(SUITE);
}
