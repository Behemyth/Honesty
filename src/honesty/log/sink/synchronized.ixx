export module synodic.honesty.log:sink.synchronized;

import :colour;
import std;
import :types;
import :sink;

namespace synodic::honesty::log
{
	export template<mutex Mutex>
	class SynchronizedSink : public Sink
	{
	public:
		explicit SynchronizedSink(LevelType level = LevelType::DEFER)
		{
		}

		~SynchronizedSink() override = default;

		void LogV(const LevelType level, const std::string_view fmt, const std::format_args args) final
		{
			std::lock_guard<Mutex> lock(mutex_);
			SynchronizedLogV(level, fmt, args);
		}

		void Flush() final
		{
			std::lock_guard<Mutex> lock(mutex_);
			SynchronizedFlush();
		}

		void SetFormatter(Formatter* formatter) final
		{
			std::lock_guard<Mutex> lock(mutex_);
			SynchronizedSetFormatter(formatter);
		}

	protected:
		virtual void SynchronizedLogV(LevelType level, std::string_view fmt, std::format_args args) = 0;

		virtual void SynchronizedFlush()
		{
		}

		virtual void SynchronizedSetFormatter(Formatter* formatter)
		{
		}

		Mutex mutex_;
	};
}
