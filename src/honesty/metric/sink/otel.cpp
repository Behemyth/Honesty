module;

#include "opentelemetry/exporters/ostream/metric_exporter_factory.h"
#include "opentelemetry/metrics/meter_provider.h"
#include "opentelemetry/metrics/provider.h"
#include "opentelemetry/sdk/metrics/export/periodic_exporting_metric_reader_factory.h"
#include "opentelemetry/sdk/metrics/meter_provider.h"
#include "opentelemetry/sdk/metrics/meter_provider_factory.h"

module synodic.honesty.metric;
import :sink.otel;
import synodic.honesty.log;

namespace honesty::metric
{
	namespace
	{
		auto& OTelMetricSinkLogger()
		{
			static log::Logger logger = []() {
				log::Logger l = log::RootLogger().CreateLogger("honesty.metric.otel");
				l.SetLevel(log::LevelType::WARNING);
				return l;
			}();
			return logger;
		}
	}

	void InitMetricExport(std::chrono::milliseconds exportIntervalMs)
	{
		OTelMetricSinkLogger().Debug("Initializing OpenTelemetry metrics export pipeline with interval: {}ms",
			exportIntervalMs.count());

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

		OTelMetricSinkLogger().Info("OpenTelemetry metrics export pipeline initialized with OStream exporter");
	}

	void ExportResults(std::string_view benchmarkName, const Results& results)
	{
		OTelMetricSinkLogger().Debug("Exporting benchmark results for '{}'", benchmarkName);

		auto provider = opentelemetry::metrics::Provider::GetMeterProvider();
		auto meter    = provider->GetMeter("honesty.metric", "1.0.0");

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

		OTelMetricSinkLogger().Trace("Exported benchmark '{}': mean={}ns, iterations={}, throughput={} ops/s",
			benchmarkName, results.mean.count(), results.iterations, results.iterations_per_second);
	}

	void CleanupMetricExport()
	{
		OTelMetricSinkLogger().Debug("Cleaning up OpenTelemetry metrics export pipeline");

		std::shared_ptr<opentelemetry::metrics::MeterProvider> noop;
		opentelemetry::metrics::Provider::SetMeterProvider(noop);

		OTelMetricSinkLogger().Info("OpenTelemetry metrics export pipeline cleaned up");
	}
}
