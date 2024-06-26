import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	// Compilation of the test suite tests indirect name construction. Unfortunately, it's an implicit test.
	constexpr Suite SUITE(
		"suite",
		[]() -> Generator
		{
			co_yield "name"_test = [&](const Requirements& requirements)
			{
				{
					constexpr Suite testSuite(
						"test",
						[]() -> Generator
						{
							co_return;
						});

					const auto name = testSuite.Name();
					requirements.ExpectEquals(name, "test");
				}

				requirements.ExpectThrow<std::runtime_error>()
			};
		});

	SuiteRegistrar _(SUITE);
}
