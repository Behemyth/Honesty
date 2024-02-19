export module synodic.honesty.terminal:terminal;

import std;

export namespace synodic::honesty::terminal
{
	struct Uint8Color {
  std::uint8_t code;
};

struct Uint24Color {
  std::uint8_t red;
  std::uint8_t green;
  std::uint8_t blue;
};
}