import std;
import synodic.honesty.test;
import synodic.honesty.utility;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"inplace_string",
		[]() -> Generator
		{
			co_yield "construction"_test = [](const Requirements& requirements)
			{
				{
					synodic::honesty::utility::InplaceString str('t', 'e', 's', 't');

					requirements.ExpectEquals(str.size(), 4);
					requirements.ExpectEquals(str.view(), "test");
				}
				{
					synodic::honesty::utility::InplaceString str("test");
					requirements.ExpectEquals(str.size(), 4);
					requirements.ExpectEquals(str.view(), "test");
				}
				{
					synodic::honesty::utility::InplaceString<5> str(std::string_view("test"));
					requirements.ExpectEquals(str.size(), 4);
					requirements.ExpectEquals(str.view(), "test");
				}
			};

			co_yield "assignment"_test = [](const Requirements& requirements)
			{
				synodic::honesty::utility::InplaceString<4> str("bad");

				str = "test";

				requirements.ExpectEquals(str.size(), 4);
				requirements.ExpectEquals(str.view(), "test");
			};
		});
	SuiteRegistrar _(SUITE);
}
