import std;
import synodic.honesty.metric;
import synodic.honesty.utility;
import synodic.honesty.test;

using namespace honesty::test;
using namespace honesty::test::literals;
using namespace honesty::metric;

namespace
{
	// Helper to create test Results
	auto MakeResults(double meanNs, std::size_t iterations = 1000) -> Results
	{
		Results results;
		results.iterations = iterations;
		results.samples = 100;
		results.totalDuration = Duration(static_cast<long long>(meanNs * static_cast<double>(iterations)));
		results.mean = std::chrono::duration<double, std::nano>(meanNs);
		results.variance = std::chrono::duration<double, std::nano>(meanNs * 0.1);
		results.stddev = std::chrono::duration<double, std::nano>(std::sqrt(meanNs * 0.1));
		results.min = std::chrono::duration<double, std::nano>(meanNs * 0.8);
		results.max = std::chrono::duration<double, std::nano>(meanNs * 1.2);
		results.iterations_per_second = 1e9 / meanNs;
		return results;
	}

	Suite SUITE(
		"benchmark_history",
		[](const Fixture& fixture) -> Generator
		{
			co_yield "record_and_retrieve"_test = [&](const Requirements& requirements)
			{
				const auto historyPath = fixture.SuiteDirectory() / "test_history.json";

				HistoryConfig config;
				config.customPath = historyPath;

				BenchmarkHistory history(config);

				const auto results = MakeResults(100.0);
				history.Record("test/benchmark", results);

				const auto retrieved = history.GetHistory("test/benchmark");

				requirements.ExpectEquals(retrieved.size(), std::size_t{1});
				requirements.Expect(std::abs(retrieved[0].meanNs - 100.0) < 0.001);
			};

			co_yield "multiple_records_newest_first"_test = [&](const Requirements& requirements)
			{
				const auto historyPath = fixture.SuiteDirectory() / "test_history_order.json";

				HistoryConfig config;
				config.customPath = historyPath;

				BenchmarkHistory history(config);

				// Record three results
				history.Record("test/benchmark", MakeResults(100.0));
				history.Record("test/benchmark", MakeResults(200.0));
				history.Record("test/benchmark", MakeResults(300.0));

				const auto retrieved = history.GetHistory("test/benchmark");

				requirements.ExpectEquals(retrieved.size(), std::size_t{3});
				// Newest (300.0) should be first
				requirements.Expect(std::abs(retrieved[0].meanNs - 300.0) < 0.001);
				requirements.Expect(std::abs(retrieved[1].meanNs - 200.0) < 0.001);
				requirements.Expect(std::abs(retrieved[2].meanNs - 100.0) < 0.001);
			};

			co_yield "max_history_limit"_test = [&](const Requirements& requirements)
			{
				const auto historyPath = fixture.SuiteDirectory() / "test_history_limit.json";

				HistoryConfig config;
				config.customPath = historyPath;
				config.maxHistorySize = 3;

				BenchmarkHistory history(config);

				// Record five results
				for (int i = 0; i < 5; ++i)
				{
					history.Record("test/benchmark", MakeResults(static_cast<double>(i + 1) * 100.0));
				}

				const auto retrieved = history.GetHistory("test/benchmark");

				// Should only keep the last 3
				requirements.ExpectEquals(retrieved.size(), std::size_t{3});
				requirements.Expect(std::abs(retrieved[0].meanNs - 500.0) < 0.001); // Newest
				requirements.Expect(std::abs(retrieved[2].meanNs - 300.0) < 0.001); // Oldest kept
			};

			co_yield "multiple_benchmarks"_test = [&](const Requirements& requirements)
			{
				const auto historyPath = fixture.SuiteDirectory() / "test_history_multi.json";

				HistoryConfig config;
				config.customPath = historyPath;

				BenchmarkHistory history(config);

				history.Record("suite/bench1", MakeResults(100.0));
				history.Record("suite/bench2", MakeResults(200.0));
				history.Record("suite/group/bench3", MakeResults(300.0));

				const auto names = history.GetBenchmarkNames();

				requirements.ExpectEquals(names.size(), std::size_t{3});

				// Verify each benchmark has its own history
				requirements.Expect(std::abs(history.GetHistory("suite/bench1")[0].meanNs - 100.0) < 0.001);
				requirements.Expect(std::abs(history.GetHistory("suite/bench2")[0].meanNs - 200.0) < 0.001);
				requirements.Expect(std::abs(history.GetHistory("suite/group/bench3")[0].meanNs - 300.0) < 0.001);
			};

			co_yield "persistence_across_instances"_test = [&](const Requirements& requirements)
			{
				const auto historyPath = fixture.SuiteDirectory() / "test_history_persist.json";

				// First instance: record data
				{
					HistoryConfig config;
					config.customPath = historyPath;

					BenchmarkHistory history(config);
					history.Record("test/benchmark", MakeResults(100.0));
				}

				// Second instance: should see the data
				{
					HistoryConfig config;
					config.customPath = historyPath;

					BenchmarkHistory history(config);
					const auto retrieved = history.GetHistory("test/benchmark");

					requirements.ExpectEquals(retrieved.size(), std::size_t{1});
					requirements.Expect(std::abs(retrieved[0].meanNs - 100.0) < 0.001);
				}
			};

			co_yield "compare_no_previous"_test = [&](const Requirements& requirements)
			{
				const auto historyPath = fixture.SuiteDirectory() / "test_compare_empty.json";

				HistoryConfig config;
				config.customPath = historyPath;

				BenchmarkHistory history(config);

				const auto result = history.Compare("nonexistent/benchmark", MakeResults(100.0));

				requirements.Expect(!result.has_value());
			};

			co_yield "compare_detects_regression"_test = [&](const Requirements& requirements)
			{
				const auto historyPath = fixture.SuiteDirectory() / "test_compare_regression.json";

				HistoryConfig config;
				config.customPath = historyPath;
				config.regressionThreshold = 0.05; // 5%

				BenchmarkHistory history(config);

				// Record baseline (100ns)
				history.Record("test/benchmark", MakeResults(100.0));

				// Compare with slower result (120ns = 20% regression)
				const auto result = history.Compare("test/benchmark", MakeResults(120.0));

				requirements.Expect(result.has_value());
				requirements.ExpectEquals(
					static_cast<int>(result->status),
					static_cast<int>(ComparisonResult::Status::Regressed));
				requirements.Expect(result->meanChangePercent > 0.15); // ~20%
			};

			co_yield "compare_detects_improvement"_test = [&](const Requirements& requirements)
			{
				const auto historyPath = fixture.SuiteDirectory() / "test_compare_improvement.json";

				HistoryConfig config;
				config.customPath = historyPath;
				config.improvementThreshold = 0.05; // 5%

				BenchmarkHistory history(config);

				// Record baseline (100ns)
				history.Record("test/benchmark", MakeResults(100.0));

				// Compare with faster result (80ns = 20% improvement)
				const auto result = history.Compare("test/benchmark", MakeResults(80.0));

				requirements.Expect(result.has_value());
				requirements.ExpectEquals(
					static_cast<int>(result->status),
					static_cast<int>(ComparisonResult::Status::Improved));
				requirements.Expect(result->meanChangePercent < -0.15); // ~-20%
			};

			co_yield "compare_unchanged_within_threshold"_test = [&](const Requirements& requirements)
			{
				const auto historyPath = fixture.SuiteDirectory() / "test_compare_unchanged.json";

				HistoryConfig config;
				config.customPath = historyPath;
				config.regressionThreshold = 0.10;	  // 10%
				config.improvementThreshold = 0.10;

				BenchmarkHistory history(config);

				// Record baseline (100ns)
				history.Record("test/benchmark", MakeResults(100.0));

				// Compare with similar result (102ns = 2% change)
				const auto result = history.Compare("test/benchmark", MakeResults(102.0));

				requirements.Expect(result.has_value());
				requirements.ExpectEquals(
					static_cast<int>(result->status),
					static_cast<int>(ComparisonResult::Status::Unchanged));
			};

			co_yield "clear_history"_test = [&](const Requirements& requirements)
			{
				const auto historyPath = fixture.SuiteDirectory() / "test_clear.json";

				HistoryConfig config;
				config.customPath = historyPath;

				BenchmarkHistory history(config);

				history.Record("test/benchmark", MakeResults(100.0));
				requirements.ExpectEquals(history.GetBenchmarkNames().size(), std::size_t{1});

				history.Clear();
				requirements.ExpectEquals(history.GetBenchmarkNames().size(), std::size_t{0});
			};
		});
	SuiteRegistrar _(SUITE);
}
