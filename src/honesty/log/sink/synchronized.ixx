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
		explicit SynchronizedSink(LevelType level = LevelType::DEFER);
		~SynchronizedSink() override = default;

		void LogV(LevelType level, std::string_view fmt, std::format_args args) final;

		void Flush() final;
		void SetFormatter(Formatter* formatter) final;

	protected:
		virtual void SynchronizedLogV(LevelType level, std::string_view fmt, std::format_args args) = 0;

		virtual void SynchronizedFlush();
		virtual void SynchronizedSetFormatter(Formatter* formatter);

		Mutex mutex_;
	};

	template<mutex Mutex>
	SynchronizedSink<Mutex>::SynchronizedSink(LevelType level)
		: Sink(level)
	{
	}

	template<mutex Mutex>
	void SynchronizedSink<Mutex>::LogV(LevelType level, std::string_view fmt, std::format_args args)
	{
		std::lock_guard<Mutex> lock(mutex_);
		SynchronizedLogV(level, fmt, args);
	}

	template<mutex Mutex>
	void SynchronizedSink<Mutex>::Flush()
	{
		std::lock_guard<Mutex> lock(mutex_);
		SynchronizedFlush();
	}

	template<mutex Mutex>
	void SynchronizedSink<Mutex>::SetFormatter(Formatter* formatter)
	{
		std::lock_guard<Mutex> lock(mutex_);
		SynchronizedSetFormatter(formatter);
	}

	template<mutex Mutex>
	void SynchronizedSink<Mutex>::SynchronizedFlush()
	{
	}

	template<mutex Mutex>
	void SynchronizedSink<Mutex>::SynchronizedSetFormatter(Formatter* formatter)
	{
	}
}
