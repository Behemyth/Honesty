module;

#include "opentelemetry/exporters/ostream/span_exporter_factory.h"
#include "opentelemetry/sdk/trace/exporter.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/trace/provider.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/tracer_provider.h"


export module synodic.honesty.trace:sink.otel;

namespace honesty::trace
{
	void Init()
	{
		auto exporter = opentelemetry::exporter::trace::OStreamSpanExporterFactory::Create();
		auto processor = opentelemetry::sdk::trace::SimpleSpanProcessorFactory::Create(std::move(exporter));

		const std::shared_ptr sdkProvider =
			opentelemetry::sdk::trace::TracerProviderFactory::Create(std::move(processor));

		// Set the global trace provider
		const std::shared_ptr<opentelemetry::trace::TracerProvider>& apiProvider = sdkProvider;
		opentelemetry::trace::Provider::SetTracerProvider(apiProvider);
	}


	void CleanupTracer()
	{
		const std::shared_ptr<opentelemetry::trace::TracerProvider> noop;
		opentelemetry::trace::Provider::SetTracerProvider(noop);
	}



}
