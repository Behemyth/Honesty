export module synodic.honesty.log:sink.scatter;

import std;
import :colour;
import :sink.synchronized;
import :types;
import :logger;

import inplace_vector;

namespace synodic::honesty::log
{

	// TODO: Share functionality when deducing this works

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

		void AddSink(Sink* input)
		{
			for (const Sink* sink: sinks_)
			{
				if (sink == input)
				{
					return;
				}
			}

			sinks_.push_back(input);
		}

	private:
		std::inplace_vector<Sink*, N> sinks_;
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

		void AddSink(Sink* input)
		{
			for (const Sink* sink: sinks_)
			{
				if (sink == input)
				{
					return;
				}
			}

			sinks_.push_back(input);
		}

	private:
		std::vector<Sink*> sinks_;
	};
}
