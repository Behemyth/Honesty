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
			co_yield "equality"_test = [](const Requirements& requirements)
			{
				constexpr synodic::honesty::utility::FixedString str("test");
				constexpr synodic::honesty::utility::FixedStringU8 u8Str(u8"test");
				constexpr synodic::honesty::utility::FixedStringU16 u16Str(u"test");
				constexpr synodic::honesty::utility::FixedStringU32 u32Str(U"test");

				constexpr synodic::honesty::utility::FixedString strCopy("test");
				constexpr synodic::honesty::utility::FixedStringU8 u8StrCopy(u8"test");
				constexpr synodic::honesty::utility::FixedStringU16 u16StrCopy(u"test");
				constexpr synodic::honesty::utility::FixedStringU32 u32StrCopy(U"test");

				requirements.ExpectEquals(str, str);
				requirements.ExpectEquals(u8Str, u8Str);
				requirements.ExpectEquals(u16Str, u16Str);
				requirements.ExpectEquals(u32Str, u32Str);

				requirements.ExpectEquals(str, strCopy);
				requirements.ExpectEquals(u8Str, u8StrCopy);
				requirements.ExpectEquals(u16Str, u16StrCopy);
				requirements.ExpectEquals(u32Str, u32StrCopy);

				requirements.ExpectEquals(str, std::string_view("test"));
				requirements.ExpectEquals(u8Str, std::u8string_view(u8"test"));
				requirements.ExpectEquals(u16Str, std::u16string_view(u"test"));
				requirements.ExpectEquals(u32Str, std::u32string_view(U"test"));

				requirements.ExpectEquals(str, "test");
				requirements.ExpectEquals(u8Str, u8"test");
				requirements.ExpectEquals(u16Str, u"test");
				requirements.ExpectEquals(u32Str, U"test");

				requirements.ExpectEquals("test", str);
				requirements.ExpectEquals(u8"test", u8Str);
				requirements.ExpectEquals(u"test", u16Str);
				requirements.ExpectEquals(U"test", u32Str);
			};

			co_yield "comparison"_test = [](const Requirements& requirements)
			{
				constexpr synodic::honesty::utility::FixedString strA("a");
				constexpr synodic::honesty::utility::FixedStringU8 u8StrA(u8"a");
				constexpr synodic::honesty::utility::FixedStringU16 u16StrA(u"a");
				constexpr synodic::honesty::utility::FixedStringU32 u32StrA(U"a");

				constexpr synodic::honesty::utility::FixedString strB("b");
				constexpr synodic::honesty::utility::FixedStringU8 u8StrB(u8"b");
				constexpr synodic::honesty::utility::FixedStringU16 u16StrB(u"b");
				constexpr synodic::honesty::utility::FixedStringU32 u32StrB(U"b");

				requirements.ExpectGreater(strA, strB);
				requirements.ExpectGreater(u8StrA, u8StrB);
				requirements.ExpectGreater(u16StrA, u16StrB);
				requirements.ExpectGreater(u32StrA, u32StrB);

				requirements.ExpectGreater(strA, "b");
				requirements.ExpectGreater(u8StrA, u8"b");
				requirements.ExpectGreater(u16StrA, u"b");
				requirements.ExpectGreater(u32StrA, U"b");
			};
		});

	SuiteRegistrar _(stringSuite);
}
