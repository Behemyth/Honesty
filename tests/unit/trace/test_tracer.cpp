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
			co_yield TODO / "global"_test = [](const Requirements& requirements)
			{
				//const honesty::trace::Tracer& tracer = honesty::trace::GetTracer();
			};

			co_yield TODO / "span"_test = [](const Requirements& requirements)
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
