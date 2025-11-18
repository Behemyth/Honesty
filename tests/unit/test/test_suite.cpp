import std;
import synodic.honesty.test;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	constexpr Suite SUITE(
		"suite",
		[](const Fixture& fixture) -> Generator
		{
			co_yield "name"_test = [&](const Requirements& requirements)
			{

			};
		});

	SuiteRegistrar _(SUITE);
}
