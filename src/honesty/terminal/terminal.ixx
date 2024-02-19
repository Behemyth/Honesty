export module synodic.honesty.terminal:terminal;

import std;

export namespace synodic::honesty::terminal
{
	struct color8_t
	{
		std::uint8_t code;
	};

	struct color24_t
	{
		std::uint8_t red;
		std::uint8_t green;
		std::uint8_t blue;
	};
}
