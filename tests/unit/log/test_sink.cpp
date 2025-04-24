import std;
import synodic.honesty.test;
import synodic.honesty.log;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	Suite SUITE(
		"otel_sink",
		[]() -> Generator
		{
			co_yield TODO / "todo"_test = [](const Requirements& requirements)
			{
				requirements.Assert(true);
			};
		});

	SuiteRegistrar _(SUITE);
}
