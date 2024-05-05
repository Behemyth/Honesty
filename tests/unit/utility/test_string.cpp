import std;
import synodic.honesty.test;
import synodic.honesty.utility;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite stringSuite(
		"string",
		[]() -> Generator
		{
			co_yield "comparison"_test = []()
			{
				constexpr synodic::honesty::utility::FixedString str("test");
				constexpr synodic::honesty::utility::FixedStringU8 u8Str(u8"test");
				constexpr synodic::honesty::utility::FixedStringU16 u16Str(u"test");
				constexpr synodic::honesty::utility::FixedStringU32 u32Str(U"test");

				ExpectEquals(str, std::string_view("test"));
				ExpectEquals(u8Str, std::u8string_view(u8"test"));
				ExpectEquals(u16Str, std::u16string_view(u"test"));
				ExpectEquals(u32Str, std::u32string_view(U"test"));

				ExpectEquals(str, "test");
				ExpectEquals(u8Str, u8"test");
				ExpectEquals(u16Str, u"test");
				ExpectEquals(u32Str, U"test");
			};
		});

	bool registered = RegisterSuite(stringSuite);
}
