export module synodic.honesty.log:sink.console;

import std;
import :colour;
import :sink;

namespace synodic::honesty::log
{
	export class Console : public Sink
	{
	public:
		Console() = default;

		void LogV(std::string_view fmt, std::format_args args) override;
		void LogV(const text_style& style, std::string_view fmt, std::format_args args) override;

	private:
	};
}
