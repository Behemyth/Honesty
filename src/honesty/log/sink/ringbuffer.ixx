export module synodic.honesty.log:sink.ringbuffer;

import std;
import :colour;
import :sink.synchronized;
import :types;

namespace synodic::honesty::log
{
	export template<mutex Mutex>
	class RingBuffer final : public SynchronizedSink<Mutex>
	{
	public:
		RingBuffer() = default;

		void SynchronizedLogV(LevelType level, std::string_view fmt, std::format_args args) override;

	private:
	};

	template<mutex Mutex>
	void RingBuffer<Mutex>::SynchronizedLogV(LevelType level, std::string_view fmt, std::format_args args)
	{
	}
}
