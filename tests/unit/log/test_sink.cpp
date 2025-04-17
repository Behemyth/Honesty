import std;
import synodic.honesty.test;
import synodic.honesty.log;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

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
