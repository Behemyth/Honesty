import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"suite",
		[]() -> Generator
		{
			co_yield "name"_test = [&](const Requirements& requirements)
			{
				constexpr std::string_view name = "test";
				Suite testSuite(
					name,
					[]() -> Generator
					{
						co_return;
					});

				requirements.ExpectEquals(testSuite.Name(), name);
			};
		});

	SuiteRegistrar _(SUITE);
}
