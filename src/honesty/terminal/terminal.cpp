module synodic.honesty.terminal:terminal;

import :terminal;
import std;

namespace synodic::honesty::terminal
{
	text_style::text_style(terminal::color8_t color) :
		fg_color(color)
	{
	}

	text_style::text_style(terminal::color24_t color) :
		fg_color(color)
	{
	}

	std::string vformat(const text_style& style, std::string_view fmt, std::format_args args)
	{
		std::string data;
		detail::vformat_to(std::back_inserter(data), style, fmt, args);
		return data;
	}

}
