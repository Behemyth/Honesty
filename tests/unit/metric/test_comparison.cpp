import std;
import synodic.honesty.metric;
import synodic.honesty.test;

using namespace honesty::test;
using namespace honesty::test::literals;
using namespace honesty::metric;

namespace
{
	Suite SUITE(
		"benchmark_comparison",
		[]() -> Generator
		{
			co_yield "format_duration_nanoseconds"_test = [](const Requirements& requirements)
			{
				const auto result = FormatDuration(123.45);
				requirements.Expect(result.find("ns") != std::string::npos);
				requirements.Expect(result.find("123") != std::string::npos);
			};

			co_yield "format_duration_microseconds"_test = [](const Requirements& requirements)
			{
				const auto result = FormatDuration(1234.5);
				requirements.Expect(result.find("µs") != std::string::npos || result.find("us") != std::string::npos);
			};

			co_yield "format_duration_milliseconds"_test = [](const Requirements& requirements)
			{
				const auto result = FormatDuration(1'234'567.0);
				requirements.Expect(result.find("ms") != std::string::npos);
			};

			co_yield "format_duration_seconds"_test = [](const Requirements& requirements)
			{
				const auto result = FormatDuration(1'234'567'890.0);
				requirements.Expect(result.find(" s") != std::string::npos);
			};

			co_yield "format_throughput_ops"_test = [](const Requirements& requirements)
			{
				const auto result = FormatThroughput(500.0);
				requirements.Expect(result.find("ops/s") != std::string::npos);
			};

			co_yield "format_throughput_kops"_test = [](const Requirements& requirements)
			{
				const auto result = FormatThroughput(50000.0);
				requirements.Expect(result.find("Kops/s") != std::string::npos);
			};

			co_yield "format_throughput_mops"_test = [](const Requirements& requirements)
			{
				const auto result = FormatThroughput(50'000'000.0);
				requirements.Expect(result.find("Mops/s") != std::string::npos);
			};

			co_yield "format_percent_positive"_test = [](const Requirements& requirements)
			{
				DisplayConfig config;
				config.useColor = false;

				const auto result = FormatPercentChange(0.25, true, config);
				requirements.Expect(result.find("+25") != std::string::npos);
				requirements.Expect(result.find("%") != std::string::npos);
			};

			co_yield "format_percent_negative"_test = [](const Requirements& requirements)
			{
				DisplayConfig config;
				config.useColor = false;

				const auto result = FormatPercentChange(-0.15, true, config);
				requirements.Expect(result.find("-15") != std::string::npos);
				requirements.Expect(result.find("%") != std::string::npos);
			};

			co_yield "status_symbol_improved"_test = [](const Requirements& requirements)
			{
				DisplayConfig config;
				config.useColor = false;
				config.useUnicode = true;

				const auto result = GetStatusSymbol(ComparisonResult::Status::Improved, config);
				requirements.Expect(!result.empty());
			};

			co_yield "status_symbol_regressed"_test = [](const Requirements& requirements)
			{
				DisplayConfig config;
				config.useColor = false;
				config.useUnicode = true;

				const auto result = GetStatusSymbol(ComparisonResult::Status::Regressed, config);
				requirements.Expect(!result.empty());
			};

			co_yield "status_symbol_unchanged"_test = [](const Requirements& requirements)
			{
				DisplayConfig config;
				config.useColor = false;
				config.useUnicode = true;

				const auto result = GetStatusSymbol(ComparisonResult::Status::Unchanged, config);
				requirements.Expect(!result.empty());
			};

			co_yield "status_text"_test = [](const Requirements& requirements)
			{
				DisplayConfig config;
				config.useColor = false;

				requirements.Expect(GetStatusText(ComparisonResult::Status::Improved, config).find("FASTER") != std::string::npos);
				requirements.Expect(GetStatusText(ComparisonResult::Status::Regressed, config).find("SLOWER") != std::string::npos);
				requirements.Expect(GetStatusText(ComparisonResult::Status::Unchanged, config).find("SAME") != std::string::npos);
			};

			co_yield "format_comparison_result"_test = [](const Requirements& requirements)
			{
				ComparisonResult result;
				result.benchmarkName = "test/benchmark";
				result.previous.meanNs = 100.0;
				result.previous.stddevNs = 10.0;
				result.previous.iterationsPerSecond = 10'000'000.0;
				result.current.meanNs = 80.0;
				result.current.stddevNs = 8.0;
				result.current.iterationsPerSecond = 12'500'000.0;
				result.current.samples = 100;
				result.current.iterations = 1000;
				result.meanChangePercent = -0.20;
				result.stddevChangePercent = -0.20;
				result.throughputChangePercent = 0.25;
				result.status = ComparisonResult::Status::Improved;

				DisplayConfig config;
				config.useColor = false;

				const auto output = FormatComparison(result, config);

				requirements.Expect(output.find("test/benchmark") != std::string::npos);
				requirements.Expect(output.find("Mean") != std::string::npos);
				requirements.Expect(output.find("Throughput") != std::string::npos);
			};

			co_yield "format_comparison_header"_test = [](const Requirements& requirements)
			{
				DisplayConfig config;
				config.useColor = false;

				const auto output = FormatComparisonHeader(config);

				requirements.Expect(output.find("Benchmark") != std::string::npos);
				requirements.Expect(output.find("Previous") != std::string::npos);
				requirements.Expect(output.find("Current") != std::string::npos);
				requirements.Expect(output.find("Change") != std::string::npos);
			};

			co_yield "format_comparison_summary_empty"_test = [](const Requirements& requirements)
			{
				DisplayConfig config;
				config.useColor = false;

				std::vector<ComparisonResult> results;
				const auto output = FormatComparisonSummary(results, config);

				requirements.Expect(output.find("No benchmark") != std::string::npos);
			};

			co_yield "format_comparison_summary_counts"_test = [](const Requirements& requirements)
			{
				DisplayConfig config;
				config.useColor = false;

				std::vector<ComparisonResult> results;

				ComparisonResult improved;
				improved.benchmarkName = "improved";
				improved.status = ComparisonResult::Status::Improved;
				improved.previous.meanNs = 100.0;
				improved.current.meanNs = 80.0;
				improved.meanChangePercent = -0.20;
				results.push_back(improved);

				ComparisonResult regressed;
				regressed.benchmarkName = "regressed";
				regressed.status = ComparisonResult::Status::Regressed;
				regressed.previous.meanNs = 100.0;
				regressed.current.meanNs = 120.0;
				regressed.meanChangePercent = 0.20;
				results.push_back(regressed);

				ComparisonResult unchanged;
				unchanged.benchmarkName = "unchanged";
				unchanged.status = ComparisonResult::Status::Unchanged;
				unchanged.previous.meanNs = 100.0;
				unchanged.current.meanNs = 101.0;
				unchanged.meanChangePercent = 0.01;
				results.push_back(unchanged);

				const auto output = FormatComparisonSummary(results, config);

				requirements.Expect(output.find("Summary") != std::string::npos);
				requirements.Expect(output.find("1 improved") != std::string::npos);
				requirements.Expect(output.find("1 regressed") != std::string::npos);
				requirements.Expect(output.find("1 unchanged") != std::string::npos);
			};

			co_yield "format_history_trend"_test = [](const Requirements& requirements)
			{
				DisplayConfig config;
				config.useColor = false;

				std::vector<HistoricalEntry> history;

				HistoricalEntry entry1;
				entry1.timestamp = "2025-01-03T12:00:00";
				entry1.meanNs = 100.0;
				entry1.iterationsPerSecond = 10'000'000.0;
				history.push_back(entry1);

				HistoricalEntry entry2;
				entry2.timestamp = "2025-01-02T12:00:00";
				entry2.meanNs = 110.0;
				entry2.iterationsPerSecond = 9'090'909.0;
				history.push_back(entry2);

				HistoricalEntry entry3;
				entry3.timestamp = "2025-01-01T12:00:00";
				entry3.meanNs = 120.0;
				entry3.iterationsPerSecond = 8'333'333.0;
				history.push_back(entry3);

				const auto output = FormatHistoryTrend("test/benchmark", history, 5, config);

				requirements.Expect(output.find("test/benchmark") != std::string::npos);
				requirements.Expect(output.find("2025-01-03") != std::string::npos);
				requirements.Expect(output.find("2025-01-02") != std::string::npos);
				requirements.Expect(output.find("2025-01-01") != std::string::npos);
			};
		});
	SuiteRegistrar _(SUITE);
}
