import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	auto empty = []()
	{
	};

	auto dummy = []()
	{
		//Tag("test", "skip") / "inner"_test = emptyGenerator;
		//skip / "test"_tag / "inner"_test = emptyGenerator;
	};

	auto tagSuite = []() -> Generator
	{
		// Verify the count of tags for each test. The two tests each have two tags
		co_yield "tag"_test = [](const Requirements& requirements)
		{
			// expect_equals(test.Tags().size(), 2);
		};
	};

	Suite suite("tag", tagSuite);
	SuiteRegistrar _(suite);
}
