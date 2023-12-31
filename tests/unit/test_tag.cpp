import std;
import synodic.honesty.test;
import synodic.honesty.test.backend;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto emptyGenerator = []() -> TestGenerator
	{
		co_return;
	};

	auto dummyGenerator = []() -> TestGenerator
	{
		co_yield Tag("test", "skip") / "inner"_test = emptyGenerator;
		co_yield skip / "test"_tag / "inner"_test	= emptyGenerator;
	};

	auto tagSuite = []() -> TestGenerator
	{
		// Verify the count of tags for each test. The two tests each have two tags
		co_yield "tag"_test = []()
		{
			for (auto&& test: dummyGenerator())
			{
				expect_equals(test.Tags().size(), 2);
			}
		};
	};

	constinit Suite suite("tag Suite", tagSuite);
	bool registered = suite.Register();
}
