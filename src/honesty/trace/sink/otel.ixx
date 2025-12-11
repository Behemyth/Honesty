export module synodic.honesty.trace:sink.otel;

import std;
import :types;
import :span;

namespace honesty::trace
{
	// Forward declaration of implementation class
	class OTelSinkImpl;

	/**
	 * @brief OpenTelemetry trace sink
	 *
	 * Exports spans to OpenTelemetry-compatible backends.
	 * Uses PIMPL pattern to keep OpenTelemetry headers out of module interface.
	 */
	export class OTelSink final
	{
	public:
		explicit OTelSink(std::string_view serviceName = "honesty");
		~OTelSink();

		OTelSink(const OTelSink&) = delete;
		OTelSink& operator=(const OTelSink&) = delete;
		OTelSink(OTelSink&&) noexcept;
		OTelSink& operator=(OTelSink&&) noexcept;

		/**
		 * @brief Initialize with ostream exporter (for testing/debugging)
		 */
		void InitWithOStreamExporter();

		/**
		 * @brief Export spans to OpenTelemetry
		 */
		void Export(std::span<const SpanData> spans);

		/**
		 * @brief Flush any buffered spans
		 */
		void Flush();

	private:
		std::unique_ptr<OTelSinkImpl> impl_;
	};

	/**
	 * @brief Initialize global OpenTelemetry tracing with ostream exporter
	 */
	export void InitOTelTracing();

	/**
	 * @brief Cleanup global OpenTelemetry tracing
	 */
	export void CleanupOTelTracing();
}
