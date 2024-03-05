module synodic.honesty.terminal:terminal;

import :terminal;
import std;

namespace synodic::honesty::terminal
{
	text_style::text_style(terminal::color8_t color) :
		foreground_(color)
	{
	}

	text_style::text_style(terminal::color24_t color) :
		foreground_(color)
	{
	}

	std::optional<color_type> text_style::Foreground() const
	{
		return foreground_;
	}

	std::optional<color_type> text_style::Background() const
	{
		return background_;
	}

	std::optional<attribute> text_style::Attribute() const
	{
		return attribute_;
	}

	std::string vformat(const text_style& style, std::string_view fmt, std::format_args args)
	{
		std::string data;
		detail::vformat_to(std::back_inserter(data), style, fmt, args);
		return data;
	}

}
