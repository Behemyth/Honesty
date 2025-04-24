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
			co_yield "root_span"_test = [](const Requirements& requirements)
			{
				honesty::trace::Tracer::Get("root");
			};

			co_yield "attach_span"_test = [](const Requirements& requirements)
			{
				/*honesty::trace::Tracer tracer;

				{
					auto span = tracer.AttachSpan();
				}*/
			};
		});
	SuiteRegistrar _(SUITE);
}
