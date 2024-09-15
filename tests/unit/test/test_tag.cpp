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
			co_yield "traits"_test = [](const Requirements& requirements)
			{
				constexpr Tag tag("one");

				const int size = sizeof(Tag);
				requirements.ExpectLessEqual(size, 128);
			};

			// TODO: Do we support error descriptions on tags?

			co_yield "construction"_test = [](const Requirements& requirements)
			{
				constexpr Tag tag("one");

				requirements.Expect(tag.Size());
			};

			co_yield "range"_test = [](const Requirements& requirements)
			{
				for (constexpr Tag tags("one"); std::string_view tag: tags)
				{
					requirements.Expect(not tag.empty());
				}
			};

			co_yield "equality"_test = [](const Requirements& requirements)
			{
				{
					const auto tag = SKIP;

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag == "skip");
					requirements.Expect(tag != "run");
				}

				{
					const auto tag = Tag("test") / Tag("skip");

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag == "test");
					requirements.Expect(tag != "run");
				}

				{
					const auto tag = Tag("test") / SKIP;

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag == "test");
					requirements.Expect(tag != "run");
				}

				{
					const auto tag = SKIP / Tag("test");

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag == "test");
					requirements.Expect(tag != "run");
				}

				{
					const auto tag = SKIP / SKIP;

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag != "test");
					requirements.Expect(tag != "run");
				}
			};

			co_yield "test"_test = [](const Requirements& requirements)
			{
				{
					const Test test = SKIP / "test"_tag / "inner"_test = [](const Requirements&)
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
