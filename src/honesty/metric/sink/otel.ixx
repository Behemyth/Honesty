export module synodic.honesty.metric:sink.otel;

import :results;
import :resource;
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
	export void InitMetricExport(std::chrono::milliseconds exportIntervalMs = std::chrono::milliseconds(5000));

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
	export void ExportResults(std::string_view benchmarkName, const Results& results);

	/**
	 * @brief Export a resource sample to OpenTelemetry.
	 *
	 * Records the resource sample as OpenTelemetry metrics:
	 * - Gauge: process.cpu.usage (CPU usage percentage)
	 * - Gauge: process.memory.usage (memory usage in bytes)
	 * - Gauge: process.memory.peak (peak memory usage in bytes)
	 *
	 * @param contextName Name/identifier for the measurement context (e.g., benchmark name)
	 * @param sample The resource sample to export
	 */
	export void ExportResourceSample(std::string_view contextName, const ResourceSample& sample);

	/**
	 * @brief Export benchmark results with associated resource samples to OpenTelemetry.
	 *
	 * Convenience function that exports both benchmark timing results and
	 * resource usage in a single call, with correlated labels.
	 *
	 * @param benchmarkName Name/identifier for the benchmark
	 * @param results The benchmark results to export
	 * @param startSample Resource sample taken at benchmark start
	 * @param endSample Resource sample taken at benchmark end
	 */
	export void ExportResultsWithResources(
		std::string_view benchmarkName,
		const Results& results,
		const ResourceSample& startSample,
		const ResourceSample& endSample);

	/**
	 * @brief Cleanup the OpenTelemetry metrics export pipeline.
	 *
	 * Resets the global meter provider. Call at application shutdown.
	 */
	export void CleanupMetricExport();
}
