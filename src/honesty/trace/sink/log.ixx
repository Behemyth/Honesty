export module synodic.honesty.trace:sink.log;

import std;

import :types;
import :span;

namespace honesty::trace
{
	/**
	 * @brief Simple logging sink that writes span data to an output stream (for debugging)
	 */
	export class LogSink final
	{
	public:
		explicit LogSink(std::ostream& output = std::cout) :
			output_(output)
		{
		}

		void Export(std::span<const SpanData> spans)
		{
			for (const auto& span : spans)
			{
				output_ << "[SPAN] " << span.name;

				// Duration calculation
				auto duration = span.endTime - span.startTime;
				auto durationNs = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

				output_ << " duration=" << durationNs << "ns";
				output_ << " status=" << StatusToString(span.status);

				if (!span.statusMessage.empty())
				{
					output_ << " message=\"" << span.statusMessage << "\"";
				}

				// Attributes
				auto attrs = span.Attributes();
				if (!attrs.empty())
				{
					output_ << " attrs={";
					bool first = true;
					for (const auto& attr : attrs)
					{
						if (!first) output_ << ", ";
						first = false;
						output_ << attr.key << "=";
						PrintAttributeValue(attr.value);
					}
					output_ << "}";
				}

				output_ << "\n";
			}
		}

		void Flush()
		{
			output_.flush();
		}

	private:
		static constexpr std::string_view StatusToString(SpanStatus status)
		{
			switch (status)
			{
				case SpanStatus::UNSET: return "UNSET";
				case SpanStatus::OK: return "OK";
				case SpanStatus::ERROR: return "ERROR";
			}
			return "UNKNOWN";
		}

		void PrintAttributeValue(const AttributeValue& value)
		{
			std::visit([this](const auto& v) {
				using T = std::decay_t<decltype(v)>;
				if constexpr (std::is_same_v<T, bool>)
				{
					output_ << (v ? "true" : "false");
				}
				else if constexpr (std::is_same_v<T, std::string_view> || std::is_same_v<T, std::string>)
				{
					output_ << "\"" << v << "\"";
				}
				else
				{
					output_ << v;
				}
			}, value);
		}

		std::ostream& output_;
	};
}
