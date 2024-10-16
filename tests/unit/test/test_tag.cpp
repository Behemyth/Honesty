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
				// We want the tag size to fit within a nice, small size
				constexpr int size = sizeof(Tag);
				requirements.ExpectLessEqual(size, 128);
			};

			// TODO: Do we support error descriptions on tags?

			co_yield "construction"_test = [](const Requirements& requirements)
			{
				{
					constexpr Tag tag('t', 'e', 's', 't');

					requirements.Expect(tag.Size());

					constexpr auto data = tag.View()[0];

					requirements.Expect(data == "test");
				}

				{
					constexpr Tag tag("test");

					requirements.Expect(tag.Size());

					constexpr auto data = tag.View()[0];

					requirements.Expect(data == "test");
				}

				{
					constexpr Tag tag(std::string_view("test"));

					requirements.Expect(tag.Size());

					constexpr auto data = tag.View()[0];

					requirements.Expect(data == "test");
				}
			};

			co_yield "range"_test = [](const Requirements& requirements)
			{
				for (constexpr Tag tags("test"); std::string_view tag: tags)
				{
					requirements.Expect(not tag.empty());
				}
			};

			co_yield "equality"_test = [](const Requirements& requirements)
			{
				{
					constexpr auto tag = SKIP;

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag == "skip");
					requirements.Expect(tag != "run");
				}

				{
					constexpr auto tag = Tag("test") / Tag("skip");

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag == "test");
					requirements.Expect(tag != "run");
				}

				{
					constexpr auto tag = Tag("test") / SKIP;

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag == "test");
					requirements.Expect(tag != "run");
				}

				{
					constexpr auto tag = SKIP / Tag("test");

					requirements.Expect(tag == SKIP);
					requirements.Expect(tag == "test");
					requirements.Expect(tag != "run");
				}

				{
					constexpr auto tag = SKIP / SKIP;

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

			co_yield FAIL / "fail"_test = [](const Requirements& requirements)
			{
				requirements.Expect(false);
			};

			co_yield SKIP / "skip"_test = [](const Requirements& requirements)
			{
				requirements.Expect(false);
			};
		});
	SuiteRegistrar _(SUITE);
}
