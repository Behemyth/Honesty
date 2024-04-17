export module synodic.honesty.log:sink;

import :colour;
import std;
import :types;

namespace synodic::honesty::log
{
	export class Sink
	{
	public:
		explicit Sink(Level level = Level::DEFER);
		virtual ~Sink() = default;

		template<class... Args>
		inline void Log(Level level, std::format_string<Args...> fmt, Args&&... args)
		{
			LogV(level, fmt.get(), std::make_format_args(args...));
		}

		virtual void LogV(Level level, std::string_view fmt, std::format_args args) = 0;

		virtual void Flush();

		Level GetLevel() const;

	private:
		Level level_;
	};

}
