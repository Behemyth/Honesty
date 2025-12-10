module;

#include "opentelemetry/exporters/ostream/metric_exporter_factory.h"
#include "opentelemetry/metrics/meter_provider.h"
#include "opentelemetry/metrics/provider.h"
#include "opentelemetry/sdk/metrics/export/periodic_exporting_metric_reader_factory.h"
#include "opentelemetry/sdk/metrics/meter_provider.h"
#include "opentelemetry/sdk/metrics/meter_provider_factory.h"

export module synodic.honesty.metric:sink.otel;

import :results;
import std;

namespace honesty::metric
{
	/**
	 * @brief Initialize the OpenTelemetry metrics export pipeline.
	 *
	 * Sets up an OStream exporter with periodic export for debugging.
	 * Call this once at application startup before recording any metrics.
	 *
	 * @param exportIntervalMs Interval between metric exports in milliseconds
	 */
	export void InitMetricExport(std::chrono::milliseconds exportIntervalMs = std::chrono::milliseconds(5000))
	{
		auto exporter = opentelemetry::exporter::metrics::OStreamMetricExporterFactory::Create();

		opentelemetry::sdk::metrics::PeriodicExportingMetricReaderOptions options;
		options.export_interval_millis = exportIntervalMs;

		auto reader =
			opentelemetry::sdk::metrics::PeriodicExportingMetricReaderFactory::Create(std::move(exporter), options);

		std::shared_ptr<opentelemetry::sdk::metrics::MeterProvider> provider =
			opentelemetry::sdk::metrics::MeterProviderFactory::Create();
		provider->AddMetricReader(std::move(reader));

		std::shared_ptr<opentelemetry::metrics::MeterProvider> apiProvider = provider;
		opentelemetry::metrics::Provider::SetMeterProvider(apiProvider);
	}

	/**
	 * @brief Export benchmark results to OpenTelemetry.
	 *
	 * Records the benchmark results as OpenTelemetry metrics:
	 * - Histogram: benchmark.duration (mean iteration time in nanoseconds)
	 * - Counter: benchmark.iterations (total iteration count)
	 *
	 * @param benchmarkName Name/identifier for the benchmark
	 * @param results The benchmark results to export
	 */
	export void ExportResults(std::string_view benchmarkName, const Results& results)
	{
		auto provider = opentelemetry::metrics::Provider::GetMeterProvider();
		auto meter	  = provider->GetMeter("honesty.metric", "1.0.0");

		// Record duration distribution as histogram
		auto histogram = meter->CreateDoubleHistogram("benchmark.duration", "Benchmark iteration duration", "ns");

		std::map<std::string, std::string> labels = {{"benchmark.name", std::string(benchmarkName)}};

		// Record the mean duration per iteration
		histogram->Record(results.mean.count(), opentelemetry::common::KeyValueIterableView{labels},
			opentelemetry::context::Context{});

		// Record iteration count as counter
		auto counter = meter->CreateUInt64Counter("benchmark.iterations", "Total benchmark iterations");
		counter->Add(results.iterations, opentelemetry::common::KeyValueIterableView{labels},
			opentelemetry::context::Context{});

		// Record throughput as gauge (using observable gauge pattern)
		auto throughputHistogram =
			meter->CreateDoubleHistogram("benchmark.throughput", "Benchmark iterations per second", "ops/s");
		throughputHistogram->Record(results.iterations_per_second,
			opentelemetry::common::KeyValueIterableView{labels}, opentelemetry::context::Context{});
	}

	/**
	 * @brief Cleanup the OpenTelemetry metrics export pipeline.
	 *
	 * Resets the global meter provider. Call at application shutdown.
	 */
	export void CleanupMetricExport()
	{
		std::shared_ptr<opentelemetry::metrics::MeterProvider> noop;
		opentelemetry::metrics::Provider::SetMeterProvider(noop);
	}

}
