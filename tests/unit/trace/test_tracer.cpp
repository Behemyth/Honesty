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
				/*constexpr honesty::trace::Provider provider("one", "two", "three");

				constexpr honesty::trace::Tracer one = provider.Get("one");

				requirements.ExpectEquals(one.Label(), "one");*/
			};

			co_yield "span"_test = [](const Requirements& requirements)
			{
			/*	constexpr honesty::trace::Provider provider("tracer");

				constexpr honesty::trace::Tracer tracer = provider.Get("tracer");*/

				{
					//honesty::Span("test_span");
				}
			};
		});
	SuiteRegistrar _(SUITE);
}
