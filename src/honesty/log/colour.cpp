module synodic.honesty.log;

import std;

namespace synodic::honesty::log
{
	std::string vformat(const text_style& style, std::string_view fmt, std::format_args args)
	{
		std::string data;
		vformat_to_impl(std::back_inserter(data), style, fmt, args);
		return data;
	}

	constexpr size_t ATTRIBUTE_COUNT = std::numeric_limits<std::underlying_type_t<attribute>>::digits;

	constexpr std::array<std::uint8_t, ATTRIBUTE_COUNT> ATTRIBUTE_ANSI_CODES = {
		1, // bold
		2, // faint
		3, // italic
		4, // underline
		5, // blink
		7, // reverse
		8, // conceal
		9, // strike
	};

	std::uint8_t ConvertIndexToAnsiCode(std::uint8_t maskIndex)
	{
		return ATTRIBUTE_ANSI_CODES[maskIndex];
	}
}
