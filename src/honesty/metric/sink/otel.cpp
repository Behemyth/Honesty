module;

#include "opentelemetry/exporters/ostream/metric_exporter_factory.h"
#include "opentelemetry/metrics/meter_provider.h"
#include "opentelemetry/metrics/provider.h"
#include "opentelemetry/sdk/metrics/export/periodic_exporting_metric_reader_factory.h"
#include "opentelemetry/sdk/metrics/meter_provider.h"
#include "opentelemetry/sdk/metrics/meter_provider_factory.h"

module synodic.honesty.metric;
import :sink.otel;
import :resource;
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

	void ExportResourceSample(std::string_view contextName, const ResourceSample& sample)
	{
		OTelMetricSinkLogger().Debug("Exporting resource sample for '{}'", contextName);

		auto provider = opentelemetry::metrics::Provider::GetMeterProvider();
		auto meter    = provider->GetMeter("honesty.metric", "1.0.0");

		std::map<std::string, std::string> labels = {{"context.name", std::string(contextName)}};

		// CPU usage as gauge (percentage)
		auto cpuGauge = meter->CreateDoubleHistogram("process.cpu.usage", "Process CPU usage", "%");
		cpuGauge->Record(sample.cpuUsagePercent, opentelemetry::common::KeyValueIterableView{labels},
			opentelemetry::context::Context{});

		// Memory usage as gauge (bytes)
		auto memoryGauge = meter->CreateUInt64Histogram("process.memory.usage", "Process memory usage", "By");
		memoryGauge->Record(sample.memoryUsageBytes, opentelemetry::common::KeyValueIterableView{labels},
			opentelemetry::context::Context{});

		// Peak memory as gauge (bytes)
		auto peakMemoryGauge = meter->CreateUInt64Histogram("process.memory.peak", "Process peak memory usage", "By");
		peakMemoryGauge->Record(sample.peakMemoryBytes, opentelemetry::common::KeyValueIterableView{labels},
			opentelemetry::context::Context{});

		OTelMetricSinkLogger().Trace("Exported resource sample '{}': cpu={:.2f}%, memory={} bytes, peak={} bytes",
			contextName, sample.cpuUsagePercent, sample.memoryUsageBytes, sample.peakMemoryBytes);
	}

	void ExportResultsWithResources(
		std::string_view benchmarkName,
		const Results& results,
		const ResourceSample& startSample,
		const ResourceSample& endSample)
	{
		OTelMetricSinkLogger().Debug("Exporting benchmark results with resources for '{}'", benchmarkName);

		auto provider = opentelemetry::metrics::Provider::GetMeterProvider();
		auto meter    = provider->GetMeter("honesty.metric", "1.0.0");

		std::map<std::string, std::string> labels = {{"benchmark.name", std::string(benchmarkName)}};

		// Export benchmark timing metrics
		auto durationHistogram = meter->CreateDoubleHistogram("benchmark.duration", "Benchmark iteration duration", "ns");
		durationHistogram->Record(results.mean.count(), opentelemetry::common::KeyValueIterableView{labels},
			opentelemetry::context::Context{});

		auto iterCounter = meter->CreateUInt64Counter("benchmark.iterations", "Total benchmark iterations");
		iterCounter->Add(results.iterations, opentelemetry::common::KeyValueIterableView{labels},
			opentelemetry::context::Context{});

		auto throughputHistogram =
			meter->CreateDoubleHistogram("benchmark.throughput", "Benchmark iterations per second", "ops/s");
		throughputHistogram->Record(results.iterations_per_second,
			opentelemetry::common::KeyValueIterableView{labels}, opentelemetry::context::Context{});

		// Export resource metrics (end sample represents final state)
		auto cpuGauge = meter->CreateDoubleHistogram("benchmark.cpu.usage", "CPU usage during benchmark", "%");
		cpuGauge->Record(endSample.cpuUsagePercent, opentelemetry::common::KeyValueIterableView{labels},
			opentelemetry::context::Context{});

		auto memoryGauge = meter->CreateUInt64Histogram("benchmark.memory.usage", "Memory usage at benchmark end", "By");
		memoryGauge->Record(endSample.memoryUsageBytes, opentelemetry::common::KeyValueIterableView{labels},
			opentelemetry::context::Context{});

		// Memory delta (can be negative if memory was freed, but we use uint64 so clamp to 0)
		std::size_t memoryDelta = endSample.memoryUsageBytes > startSample.memoryUsageBytes
			? endSample.memoryUsageBytes - startSample.memoryUsageBytes
			: 0;
		auto memoryDeltaGauge = meter->CreateUInt64Histogram("benchmark.memory.delta", "Memory change during benchmark", "By");
		memoryDeltaGauge->Record(memoryDelta, opentelemetry::common::KeyValueIterableView{labels},
			opentelemetry::context::Context{});

		auto peakMemoryGauge = meter->CreateUInt64Histogram("benchmark.memory.peak", "Peak memory during benchmark", "By");
		peakMemoryGauge->Record(endSample.peakMemoryBytes, opentelemetry::common::KeyValueIterableView{labels},
			opentelemetry::context::Context{});

		OTelMetricSinkLogger().Trace(
			"Exported benchmark '{}' with resources: mean={}ns, cpu={:.2f}%, memory_delta={} bytes",
			benchmarkName, results.mean.count(), endSample.cpuUsagePercent, memoryDelta);
	}

	void CleanupMetricExport()
	{
		OTelMetricSinkLogger().Debug("Cleaning up OpenTelemetry metrics export pipeline");

		std::shared_ptr<opentelemetry::metrics::MeterProvider> noop;
		opentelemetry::metrics::Provider::SetMeterProvider(noop);

		OTelMetricSinkLogger().Info("OpenTelemetry metrics export pipeline cleaned up");
	}
}
