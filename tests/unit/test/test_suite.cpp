import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	// Compilation of the test suite tests indirect name construction. Unfortunately, it's an implicit test.
	Suite SUITE(
		"suite",
		[]() -> Generator
		{
			co_yield "name"_test = [&](const Requirements& requirements)
			{
				//// std::string_view naming
				//{
				//	constexpr std::string_view name = "test";
				//	Suite testSuite(
				//		name,
				//		[]() -> Generator
				//		{
				//			co_return;
				//		});

				//	requirements.ExpectEquals(testSuite.Name(), name);
				//}

				//// c naming
				//{
				//	constexpr auto name = "test";
				//	Suite testSuite(
				//		name,
				//		[]() -> Generator
				//		{
				//			co_return;
				//		});

				//	requirements.ExpectEquals(testSuite.Name(), name);
				//}
			};
		});

	SuiteRegistrar _(SUITE);
}
