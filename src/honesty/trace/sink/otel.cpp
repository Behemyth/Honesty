module;

#include "opentelemetry/exporters/ostream/span_exporter_factory.h"
#include "opentelemetry/sdk/trace/exporter.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/trace/provider.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/tracer_provider.h"
#include "opentelemetry/trace/span.h"
#include "opentelemetry/trace/tracer.h"
#include "opentelemetry/nostd/shared_ptr.h"
#include "opentelemetry/common/attribute_value.h"
#include "opentelemetry/sdk/version/version.h"

#include <memory>
#include <mutex>
#include <string>
#include <span>
#include <variant>
#include <cstdint>
#include <chrono>
#include <string_view>

// Windows.h defines ERROR macro which conflicts with SpanStatus::ERROR
#ifdef ERROR
#undef ERROR
#endif

module synodic.honesty.trace;
import :sink.otel;

namespace honesty::trace
{
	namespace otel = opentelemetry;

	// Convert honesty SpanKind to OpenTelemetry SpanKind
	otel::trace::SpanKind ToOTelSpanKind(SpanKind kind)
	{
		switch (kind)
		{
			case SpanKind::INTERNAL: return otel::trace::SpanKind::kInternal;
			case SpanKind::SERVER: return otel::trace::SpanKind::kServer;
			case SpanKind::CLIENT: return otel::trace::SpanKind::kClient;
			case SpanKind::PRODUCER: return otel::trace::SpanKind::kProducer;
			case SpanKind::CONSUMER: return otel::trace::SpanKind::kConsumer;
		}
		return otel::trace::SpanKind::kInternal;
	}

	// Convert honesty SpanStatus to OpenTelemetry StatusCode
	otel::trace::StatusCode ToOTelStatusCode(SpanStatus status)
	{
		switch (status)
		{
			case SpanStatus::UNSET: return otel::trace::StatusCode::kUnset;
			case SpanStatus::OK: return otel::trace::StatusCode::kOk;
			case SpanStatus::ERROR: return otel::trace::StatusCode::kError;
		}
		return otel::trace::StatusCode::kUnset;
	}

	// PIMPL implementation class
	class OTelSinkImpl
	{
	public:
		explicit OTelSinkImpl(std::string_view serviceName) :
			serviceName_(serviceName)
		{
		}

		void InitWithOStreamExporter()
		{
			auto exporter = otel::exporter::trace::OStreamSpanExporterFactory::Create();
			auto processor = otel::sdk::trace::SimpleSpanProcessorFactory::Create(std::move(exporter));

			sdkProvider_ = otel::sdk::trace::TracerProviderFactory::Create(std::move(processor));
			otel::trace::Provider::SetTracerProvider(
				std::shared_ptr<otel::trace::TracerProvider>(sdkProvider_));

			tracer_ = otel::trace::Provider::GetTracerProvider()->GetTracer(
				serviceName_, OPENTELEMETRY_SDK_VERSION);
		}

		void Export(std::span<const SpanData> spans)
		{
			std::lock_guard lock(mutex_);

			if (!tracer_)
				return;

			for (const auto& data : spans)
			{
				ExportSpan(data);
			}
		}

		void Flush()
		{
			std::lock_guard lock(mutex_);
			if (sdkProvider_)
			{
				sdkProvider_->ForceFlush();
			}
		}

	private:
		void SetAttribute(
			const otel::nostd::shared_ptr<otel::trace::Span>& span,
			std::string_view key,
			const AttributeValue& value)
		{
			auto otelKey = otel::nostd::string_view(key.data(), key.size());

			if (auto* b = std::get_if<bool>(&value))
			{
				span->SetAttribute(otelKey, *b);
			}
			else if (auto* i = std::get_if<std::int64_t>(&value))
			{
				span->SetAttribute(otelKey, *i);
			}
			else if (auto* u = std::get_if<std::uint64_t>(&value))
			{
				span->SetAttribute(otelKey, static_cast<std::int64_t>(*u));
			}
			else if (auto* d = std::get_if<double>(&value))
			{
				span->SetAttribute(otelKey, *d);
			}
			else if (auto* sv = std::get_if<std::string_view>(&value))
			{
				span->SetAttribute(otelKey, otel::nostd::string_view(sv->data(), sv->size()));
			}
			else if (auto* s = std::get_if<std::string>(&value))
			{
				span->SetAttribute(otelKey, otel::nostd::string_view(s->data(), s->size()));
			}
		}

		void ExportSpan(const SpanData& data)
		{
			otel::trace::StartSpanOptions options;
			options.kind = ToOTelSpanKind(data.kind);
			options.start_steady_time = otel::common::SteadyTimestamp(data.startTime);

			auto span = tracer_->StartSpan(
				otel::nostd::string_view(data.name.data(), data.name.size()),
				options);

			// Add attributes
			for (const auto& attr : data.Attributes())
			{
				SetAttribute(span, attr.key, attr.value);
			}

			// Set status
			span->SetStatus(
				ToOTelStatusCode(data.status),
				std::string(data.statusMessage));

			// End the span
			otel::trace::EndSpanOptions endOptions;
			endOptions.end_steady_time = otel::common::SteadyTimestamp(data.endTime);
			span->End(endOptions);
		}

		std::string serviceName_;
		std::shared_ptr<otel::sdk::trace::TracerProvider> sdkProvider_;
		otel::nostd::shared_ptr<otel::trace::Tracer> tracer_;
		std::mutex mutex_;
	};

	// OTelSink implementation forwarding to PIMPL
	OTelSink::OTelSink(std::string_view serviceName) :
		impl_(std::make_unique<OTelSinkImpl>(serviceName))
	{
	}

	OTelSink::~OTelSink() = default;
	OTelSink::OTelSink(OTelSink&&) noexcept = default;
	OTelSink& OTelSink::operator=(OTelSink&&) noexcept = default;

	void OTelSink::InitWithOStreamExporter()
	{
		impl_->InitWithOStreamExporter();
	}

	void OTelSink::Export(std::span<const SpanData> spans)
	{
		impl_->Export(spans);
	}

	void OTelSink::Flush()
	{
		impl_->Flush();
	}

	// Global functions implementation
	void InitOTelTracing()
	{
		auto exporter = otel::exporter::trace::OStreamSpanExporterFactory::Create();
		auto processor = otel::sdk::trace::SimpleSpanProcessorFactory::Create(std::move(exporter));

		auto sdkProvider = otel::sdk::trace::TracerProviderFactory::Create(std::move(processor));
		otel::trace::Provider::SetTracerProvider(
			std::shared_ptr<otel::trace::TracerProvider>(std::move(sdkProvider)));
	}

	void CleanupOTelTracing()
	{
		otel::trace::Provider::SetTracerProvider(
			std::shared_ptr<otel::trace::TracerProvider>(nullptr));
	}
}
