export module synodic.honesty.log:sink.console;

import std;
import :colour;
import :sink;
import :types;

namespace synodic::honesty::log
{
	export class Console final : public Sink
	{
	public:
		Console() = default;

		void LogV(LevelType level, std::string_view fmt, std::format_args args) override;

	private:
	};
}
