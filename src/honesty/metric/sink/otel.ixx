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
	 * @brief Cleanup the OpenTelemetry metrics export pipeline.
	 *
	 * Resets the global meter provider. Call at application shutdown.
	 */
	export void CleanupMetricExport();
}
