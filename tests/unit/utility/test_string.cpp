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
			co_yield "equality"_test = []()
			{
				constexpr synodic::honesty::utility::FixedString str("test");
				constexpr synodic::honesty::utility::FixedStringU8 u8Str(u8"test");
				constexpr synodic::honesty::utility::FixedStringU16 u16Str(u"test");
				constexpr synodic::honesty::utility::FixedStringU32 u32Str(U"test");

				constexpr synodic::honesty::utility::FixedString strCopy("test");
				constexpr synodic::honesty::utility::FixedStringU8 u8StrCopy(u8"test");
				constexpr synodic::honesty::utility::FixedStringU16 u16StrCopy(u"test");
				constexpr synodic::honesty::utility::FixedStringU32 u32StrCopy(U"test");

				ExpectEquals(str, str);
				ExpectEquals(u8Str, u8Str);
				ExpectEquals(u16Str, u16Str);
				ExpectEquals(u32Str, u32Str);

				ExpectEquals(str, strCopy);
				ExpectEquals(u8Str, u8StrCopy);
				ExpectEquals(u16Str, u16StrCopy);
				ExpectEquals(u32Str, u32StrCopy);

				ExpectEquals(str, std::string_view("test"));
				ExpectEquals(u8Str, std::u8string_view(u8"test"));
				ExpectEquals(u16Str, std::u16string_view(u"test"));
				ExpectEquals(u32Str, std::u32string_view(U"test"));

				ExpectEquals(str, "test");
				ExpectEquals(u8Str, u8"test");
				ExpectEquals(u16Str, u"test");
				ExpectEquals(u32Str, U"test");

				ExpectEquals("test", str);
				ExpectEquals(u8"test", u8Str);
				ExpectEquals(u"test", u16Str);
				ExpectEquals(U"test", u32Str);
			};

			co_yield "comparison"_test = []()
			{
				constexpr synodic::honesty::utility::FixedString strA("a");
				constexpr synodic::honesty::utility::FixedStringU8 u8StrA(u8"a");
				constexpr synodic::honesty::utility::FixedStringU16 u16StrA(u"a");
				constexpr synodic::honesty::utility::FixedStringU32 u32StrA(U"a");

				constexpr synodic::honesty::utility::FixedString strB("b");
				constexpr synodic::honesty::utility::FixedStringU8 u8StrB(u8"b");
				constexpr synodic::honesty::utility::FixedStringU16 u16StrB(u"b");
				constexpr synodic::honesty::utility::FixedStringU32 u32StrB(U"b");

				ExpectGreater(strA, strB);
				ExpectGreater(u8StrA, u8StrB);
				ExpectGreater(u16StrA, u16StrB);
				ExpectGreater(u32StrA, u32StrB);

				ExpectGreater(strA, "b");
				ExpectGreater(u8StrA, u8"b");
				ExpectGreater(u16StrA, u"b");
				ExpectGreater(u32StrA, U"b");
			};
		});

	SuiteRegistrar _(stringSuite);
}
