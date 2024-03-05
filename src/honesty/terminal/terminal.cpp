module synodic.honesty.terminal:terminal;

import :terminal;
import std;

namespace synodic::honesty::terminal
{
	std::string vformat(const text_style& style, std::string_view fmt, std::format_args args)
	{
		std::string data;
		detail::vformat_to(std::back_inserter(data), style, fmt, args);
		return data;
	}

}
