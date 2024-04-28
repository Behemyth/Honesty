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

	auto tagSuite = []()
	{
		// Verify the count of tags for each test. The two tests each have two tags
		"tag"_test = []()
		{
			// expect_equals(test.Tags().size(), 2);
		};
	};

	Suite suite("tag", tagSuite);
	bool result = RegisterSuite(suite);
}
