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

			co_yield "construction"_test = [](const Requirements& requirements)
			{
				constexpr Tag tag("one", "two", "three");

				requirements.Expect(tag.Size());
			};

			co_yield "range"_test = [](const Requirements& requirements)
			{
				for (constexpr Tag tags("one", "two", "three"); std::string_view tag: tags)
				{
					requirements.Expect(not tag.empty());
				}
			};

			co_yield "fail"_test = [](const Requirements& requirements)
			{
			};

			co_yield "equality"_test = [](const Requirements& requirements)
			{
				// TODO: Enable with 17.11 preview 3

				//{
				//	auto tag = SKIP;

				//	requirements.ExpectEquals(tag, SKIP);
				//	requirements.ExpectEquals(tag, "skip");
				//}

				//{
				//	auto tag = Tag("test", "skip");

				//	requirements.ExpectEquals(tag, "skip");
				//	requirements.ExpectEquals(tag, "test");
				//}

				//{
				//	auto tag = Tag("test") / Tag("skip");

				//	requirements.ExpectEquals(tag, SKIP);
				//	requirements.ExpectEquals(tag, SKIP);
				//}

				//{
				//	auto tag = Tag("test") / SKIP;

				//	requirements.ExpectEquals(tag, SKIP);
				//	requirements.ExpectEquals(tag, SKIP);
				//}

				//{
				//	auto tag = SKIP / Tag("test");

				//	requirements.ExpectEquals(tag, SKIP);
				//	requirements.ExpectEquals(tag, SKIP);
				//}

				//{
				//	auto tag = SKIP / SKIP;

				//	requirements.ExpectEquals(tag, SKIP);
				//	requirements.ExpectEquals(tag, SKIP);
				//}
			};

			co_yield "test"_test = [](const Requirements& requirements)
			{
				//	Tag("test", "skip") / "inner"_test = [](const Requirements&)
				//	{
				//	};
				//	SKIP / "test"_tag / "inner"_test = [](const Requirements&)
				//	{
				//	};
				//	requirements.ExpectEquals(test.Tags().size(), 2);
			};
		});
	SuiteRegistrar _(SUITE);
}
