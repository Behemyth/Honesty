export module synodic.honesty.log:sink.scatter;

import std;
import :colour;
import :sink.synchronized;
import :types;
import :logger;

namespace synodic::honesty::log
{
	export template<mutex Mutex>
	class Scatter final : public SynchronizedSink<Mutex>
	{
	public:
		explicit Scatter()
		{
		}

		void SynchronizedLogV(const LevelType level, const std::string_view fmt, std::format_args args) override
		{
			const Message message(level, std::format(fmt, args));
		}

	private:
	};
}
