import std;
import synodic.honesty.test;
import synodic.honesty.utility;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"metric",
		[]() -> Generator
		{
			co_yield TODO / "empty"_test = [](const Requirements& requirements)
			{
				requirements.Benchmark(
					"empty metric",
					[&]()
					{
					});
			};
		});
	SuiteRegistrar _(SUITE);
}
