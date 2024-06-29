export module synodic.honesty.log:sink.ostream;

import std;
import :colour;
import :sink.synchronized;
import :types;

namespace synodic::honesty::log
{
	export template<mutex Mutex>
	class OStream final : public SynchronizedSink<Mutex>
	{
	public:
		explicit OStream(std::ostream& stream) :
			stream_(stream)
		{
		}

		void SynchronizedLogV(LevelType level, const std::string_view fmt, std::format_args args) override
		{
			stream_ << std::format(fmt, args);
		}

		void SynchronizedFlush() override
		{
			stream_.flush();
		}

	private:
		std::ostream& stream_;
	};
}
