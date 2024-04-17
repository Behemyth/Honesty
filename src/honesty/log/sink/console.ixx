export module synodic.honesty.log:sink.console;

import std;
import :colour;
import :sink;
import :types;

namespace synodic::honesty::log
{
	export class Console : public Sink
	{
	public:
		Console() = default;

		void LogV(Level level, std::string_view fmt, std::format_args args) override;

	private:
	};
}
