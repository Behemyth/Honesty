import std;
import synodic.honesty.test;
import synodic.honesty.utility;

using namespace honesty::test;
using namespace honesty::test::literals;

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
