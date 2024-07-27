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

			co_yield "equality"_test = [](const Requirements& requirements)
			{
				{
					auto tag = SKIP;

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag == "skip");
					requirements.Expect(tag != "run");
				}

				{
					auto tag = Tag("test", "skip");

					requirements.Expect(tag == "skip");
					requirements.Expect(tag == "test");
					requirements.Expect(tag != "run");
				}

				{
					auto tag = Tag("test") / Tag("skip");

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag == "test");
					requirements.Expect(tag != "run");
				}

				{
					auto tag = Tag("test") / SKIP;

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag == "test");
					requirements.Expect(tag != "run");
				}

				{
					auto tag = SKIP / Tag("test");

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag == "test");
					requirements.Expect(tag != "run");
				}

				{
					auto tag = SKIP / SKIP;

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag != "test");
					requirements.Expect(tag != "run");
				}
			};

			co_yield "test"_test = [](const Requirements& requirements)
			{
				{
					Test test = Tag("test", "skip") / "inner"_test = [](const Requirements&)
					{
					};
					requirements.ExpectEquals(test.Tags().size(), 2);
				}

				{
					Test test = SKIP / "test"_tag / "inner"_test = [](const Requirements&)
					{
					};
					requirements.ExpectEquals(test.Tags().size(), 2);
				}
			};

			co_yield "fail"_test = [](const Requirements& requirements)
			{
			};
		});
	SuiteRegistrar _(SUITE);
}
