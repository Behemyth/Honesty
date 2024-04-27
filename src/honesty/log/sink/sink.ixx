export module synodic.honesty.log:sink;

import :colour;
import std;
import :types;

namespace synodic::honesty::log
{

	export class Formatter
	{
	public:
		virtual ~Formatter()  = default;
		virtual void Format() = 0;

	private:
	};

	export class Sink
	{
	public:
		explicit constexpr Sink(LevelType level = LevelType::DEFER) :
			level_(level)
		{
		}

		virtual ~Sink() = default;

		virtual void LogV(LevelType level, std::string_view fmt, std::format_args args) = 0;

		virtual void Flush()
		{
		}

		virtual void SetFormatter(Formatter* formatter)
		{
		}

		LevelType Level() const
		{
			return level_;
		}

		void SetLevel(LevelType level)
		{
			level_ = level;
		}

	protected:
		LevelType level_;
	};

}
