import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"tag",
		[]() -> Generator
		{
			// TODO: Do we support error descriptions on tags?

			// Verify the count of tags for each test. The two tests each have two tags
			co_yield "tag"_test = [](const Requirements& requirements)
			{
				Tag("test", "skip") / "inner"_test = emptyGenerator;
				SKIP / "test"_tag / "inner"_test = emptyGenerator;
				// expect_equals(test.Tags().size(), 2);
			};
		});
	SuiteRegistrar _(SUITE);
}
