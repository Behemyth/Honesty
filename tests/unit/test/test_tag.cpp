import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	auto emptyGenerator = []() -> generator<TestBase>
	{
		co_return;
	};

	auto dummyGenerator = []() -> generator<TestBase>
	{
		co_yield Tag("test", "skip") / "inner"_test = emptyGenerator;
		co_yield skip / "test"_tag / "inner"_test	= emptyGenerator;
	};

	auto tagSuite = []() -> generator<TestBase>
	{
		// Verify the count of tags for each test. The two tests each have two tags
		co_yield "tag"_test = []()
		{
			for (auto&& test: dummyGenerator())
			{
				// expect_equals(test.Tags().size(), 2);
			}
		};
	};

	Suite suite("tag", tagSuite);
	bool result = suite.Register();
}
