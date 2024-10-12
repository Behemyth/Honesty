export module synodic.honesty.log:sink.scatter;

import std;
import :colour;
import :sink.synchronized;
import :types;
import :logger;

namespace synodic::honesty::log
{
	export template<std::size_t N>
	class FixedScatter final : public Sink
	{
	public:
		explicit FixedScatter()
		{
		}

		void LogV(const LevelType level, const std::string_view fmt, const std::format_args args) override
		{
			for (Sink* sink: sinks_)
			{
				sink->LogV(level, fmt, args);
			}
		}

	private:
		std::array<Sink*, N> sinks_;
	};

	export class Scatter final : public Sink
	{
	public:
		explicit Scatter()
		{
		}

		void LogV(const LevelType level, const std::string_view fmt, const std::format_args args) override
		{
			for (Sink* sink: sinks_)
			{
				sink->LogV(level, fmt, args);
			}
		}

	private:
		std::vector<Sink*> sinks_;
	};
}
