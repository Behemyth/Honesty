module synodic.honesty.terminal:terminal;

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

}
