module;

#include "opentelemetry/exporters/ostream/log_record_exporter_factory.h"
#include "opentelemetry/logs/provider.h"
#include "opentelemetry/sdk/logs/logger_provider.h"
#include "opentelemetry/sdk/logs/logger_provider_factory.h"
#include "opentelemetry/sdk/logs/simple_log_record_processor_factory.h"

module synodic.honesty.log;
import :sink.otel;

namespace honesty::log
{
	namespace otel = opentelemetry;

	namespace
	{
		auto& OTelLogSinkLogger()
		{
			static Logger logger = []() {
				Logger l = RootLogger().CreateLogger("honesty.log.otel");
				l.SetLevel(LevelType::WARNING);
				return l;
			}();
			return logger;
		}
	}

	// Global functions implementation
	void InitOTelLogging()
	{
		OTelLogSinkLogger().Debug("Initializing OpenTelemetry logging pipeline");

		auto exporter = otel::exporter::logs::OStreamLogRecordExporterFactory::Create();
		auto processor = otel::sdk::logs::SimpleLogRecordProcessorFactory::Create(std::move(exporter));

		auto sdkProvider = otel::sdk::logs::LoggerProviderFactory::Create(std::move(processor));
		otel::logs::Provider::SetLoggerProvider(
			std::shared_ptr<otel::logs::LoggerProvider>(std::move(sdkProvider)));

		OTelLogSinkLogger().Info("OpenTelemetry logging pipeline initialized with OStream exporter");
	}

	void CleanupOTelLogging()
	{
		OTelLogSinkLogger().Debug("Cleaning up OpenTelemetry logging pipeline");

		otel::logs::Provider::SetLoggerProvider(
			std::shared_ptr<otel::logs::LoggerProvider>(nullptr));

		OTelLogSinkLogger().Info("OpenTelemetry logging pipeline cleaned up");
	}
}
