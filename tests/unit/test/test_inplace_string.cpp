import std;
import synodic.honesty.test;
import synodic.honesty.utility;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	Suite SUITE(
		"inplace_string",
		[]() -> Generator
		{
			co_yield "construction"_test = [](const Requirements& requirements)
			{
				{
					honesty::utility::InplaceString str('t', 'e', 's', 't');

					requirements.ExpectEquals(str.size(), 4);
					requirements.ExpectEquals(str.view(), "test");
				}
				{
					honesty::utility::InplaceString str("test");
					requirements.ExpectEquals(str.size(), 4);
					requirements.ExpectEquals(str.view(), "test");
				}
				{
					honesty::utility::InplaceString<5> str(std::string_view("test"));
					requirements.ExpectEquals(str.size(), 4);
					requirements.ExpectEquals(str.view(), "test");
				}
			};

			co_yield "assignment"_test = [](const Requirements& requirements)
			{
				honesty::utility::InplaceString<4> str("bad");

				str = "test";

				requirements.ExpectEquals(str.size(), 4);
				requirements.ExpectEquals(str.view(), "test");
			};
		});
	SuiteRegistrar _(SUITE);
}
