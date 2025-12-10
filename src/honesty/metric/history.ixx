/**
 * @file history.ixx
 * @brief Benchmark history persistence and comparison utilities
 *
 * Provides functionality to store benchmark results to a consolidated JSON file,
 * load historical results, and compare current runs against previous ones.
 */

export module synodic.honesty.metric:history;

import :results;
import :types;
import synodic.honesty.utility;

import std;

namespace honesty::metric
{
	/**
	 * @brief Configuration for benchmark history storage and comparison
	 */
	export struct HistoryConfig
	{
		std::size_t maxHistorySize = 20;		   ///< Maximum number of historical runs to keep per benchmark
		double regressionThreshold = 0.05;		   ///< Percentage threshold for flagging regressions (5%)
		double improvementThreshold = 0.05;		   ///< Percentage threshold for flagging improvements (5%)
		std::filesystem::path customPath {};	   ///< Custom path for history file (empty = use default)
	};

	/**
	 * @brief A single historical benchmark entry with timestamp
	 */
	export struct HistoricalEntry
	{
		std::string timestamp;				///< ISO 8601 timestamp of when the benchmark was run
		std::size_t iterations {};			///< Number of iterations
		std::size_t samples {};				///< Number of samples
		double totalDurationNs {};			///< Total duration in nanoseconds
		double meanNs {};					///< Mean time per iteration in nanoseconds
		double varianceNs {};				///< Variance in nanoseconds
		double stddevNs {};					///< Standard deviation in nanoseconds
		double minNs {};					///< Minimum time in nanoseconds
		double maxNs {};					///< Maximum time in nanoseconds
		double iterationsPerSecond {};		///< Throughput
	};

	/**
	 * @brief Comparison result between two benchmark runs
	 */
	export struct ComparisonResult
	{
		std::string benchmarkName;
		HistoricalEntry current;
		HistoricalEntry previous;
		double meanChangePercent {};		///< Positive = slower (regression), negative = faster (improvement)
		double stddevChangePercent {};
		double throughputChangePercent {};	///< Positive = better, negative = worse

		enum class Status
		{
			Improved,	///< Significantly faster
			Regressed,	///< Significantly slower
			Unchanged	///< Within threshold
		} status = Status::Unchanged;
	};

	/**
	 * @brief Manages benchmark history persistence and comparison
	 */
	export class BenchmarkHistory
	{
	public:
		explicit BenchmarkHistory(HistoryConfig config = {}) :
			config_(std::move(config))
		{
		}

		/**
		 * @brief Record a benchmark result to history
		 *
		 * @param benchmarkName Full hierarchical name (e.g., "suite/group/benchmark")
		 * @param results The benchmark results to record
		 */
		void Record(const std::string& benchmarkName, const Results& results)
		{
			LoadHistory();

			HistoricalEntry entry;
			entry.timestamp = CurrentTimestamp();
			entry.iterations = results.iterations;
			entry.samples = results.samples;
			entry.totalDurationNs = static_cast<double>(results.totalDuration.count());
			entry.meanNs = results.mean.count();
			entry.varianceNs = results.variance.count();
			entry.stddevNs = results.stddev.count();
			entry.minNs = results.min.count();
			entry.maxNs = results.max.count();
			entry.iterationsPerSecond = results.iterations_per_second;

			// Add to history (newest first)
			auto& benchmarkHistory = history_[benchmarkName];
			benchmarkHistory.insert(benchmarkHistory.begin(), entry);

			// Trim to max size
			if (benchmarkHistory.size() > config_.maxHistorySize)
			{
				benchmarkHistory.resize(config_.maxHistorySize);
			}

			SaveHistory();
		}

		/**
		 * @brief Get historical entries for a benchmark
		 *
		 * @param benchmarkName The benchmark name to look up
		 * @return Vector of historical entries (newest first), empty if not found
		 */
		auto GetHistory(const std::string& benchmarkName) const -> std::vector<HistoricalEntry>
		{
			const_cast<BenchmarkHistory*>(this)->LoadHistory();

			if (auto it = history_.find(benchmarkName); it != history_.end())
			{
				return it->second;
			}
			return {};
		}

		/**
		 * @brief Compare current results against the previous run
		 *
		 * @param benchmarkName The benchmark name
		 * @param current The current results
		 * @return Comparison result, or nullopt if no previous run exists
		 */
		auto Compare(const std::string& benchmarkName, const Results& current) -> std::optional<ComparisonResult>
		{
			LoadHistory();

			auto it = history_.find(benchmarkName);
			if (it == history_.end() || it->second.empty())
			{
				return std::nullopt;
			}

			const auto& previous = it->second.front();

			ComparisonResult result;
			result.benchmarkName = benchmarkName;
			result.previous = previous;

			// Create current entry
			result.current.timestamp = CurrentTimestamp();
			result.current.iterations = current.iterations;
			result.current.samples = current.samples;
			result.current.totalDurationNs = static_cast<double>(current.totalDuration.count());
			result.current.meanNs = current.mean.count();
			result.current.varianceNs = current.variance.count();
			result.current.stddevNs = current.stddev.count();
			result.current.minNs = current.min.count();
			result.current.maxNs = current.max.count();
			result.current.iterationsPerSecond = current.iterations_per_second;

			// Calculate percentage changes
			if (previous.meanNs > 0)
			{
				result.meanChangePercent = (result.current.meanNs - previous.meanNs) / previous.meanNs;
			}
			if (previous.stddevNs > 0)
			{
				result.stddevChangePercent = (result.current.stddevNs - previous.stddevNs) / previous.stddevNs;
			}
			if (previous.iterationsPerSecond > 0)
			{
				result.throughputChangePercent =
					(result.current.iterationsPerSecond - previous.iterationsPerSecond) / previous.iterationsPerSecond;
			}

			// Determine status based on mean time change
			if (result.meanChangePercent > config_.regressionThreshold)
			{
				result.status = ComparisonResult::Status::Regressed;
			}
			else if (result.meanChangePercent < -config_.improvementThreshold)
			{
				result.status = ComparisonResult::Status::Improved;
			}
			else
			{
				result.status = ComparisonResult::Status::Unchanged;
			}

			return result;
		}

		/**
		 * @brief Get all benchmark names in history
		 */
		auto GetBenchmarkNames() const -> std::vector<std::string>
		{
			const_cast<BenchmarkHistory*>(this)->LoadHistory();

			std::vector<std::string> names;
			names.reserve(history_.size());
			for (const auto& [name, _]: history_)
			{
				names.push_back(name);
			}
			return names;
		}

		/**
		 * @brief Clear all history
		 */
		void Clear()
		{
			history_.clear();
			SaveHistory();
		}

		/**
		 * @brief Get the path to the history file
		 */
		auto HistoryFilePath() const -> std::filesystem::path
		{
			if (!config_.customPath.empty())
			{
				return config_.customPath;
			}
			return utility::BenchmarkDirectory() / "benchmarks.json";
		}

	private:
		void LoadHistory()
		{
			if (loaded_)
			{
				return;
			}

			const auto path = HistoryFilePath();

			if (!std::filesystem::exists(path))
			{
				loaded_ = true;
				return;
			}

			std::ifstream file(path);
			if (!file)
			{
				loaded_ = true;
				return;
			}

			try
			{
				auto json = utility::JSON::Parse(file);

				if (!json.IsObject())
				{
					loaded_ = true;
					return;
				}

				for (const auto& benchmarkName: json.Keys())
				{
					const auto& benchmarkArray = json[benchmarkName];
					if (!benchmarkArray.IsArray())
					{
						continue;
					}

					std::vector<HistoricalEntry> entries;
					for (std::size_t i = 0; i < benchmarkArray.Size(); ++i)
					{
						const auto& entryJson = benchmarkArray[i];
						if (!entryJson.IsObject())
						{
							continue;
						}

						HistoricalEntry entry;

						if (entryJson.Contains("timestamp") && entryJson["timestamp"].IsString())
						{
							entry.timestamp = entryJson["timestamp"].GetString();
						}
						if (entryJson.Contains("iterations") && entryJson["iterations"].IsNumber())
						{
							entry.iterations = static_cast<std::size_t>(entryJson["iterations"].GetNumber());
						}
						if (entryJson.Contains("samples") && entryJson["samples"].IsNumber())
						{
							entry.samples = static_cast<std::size_t>(entryJson["samples"].GetNumber());
						}
						if (entryJson.Contains("totalDurationNs") && entryJson["totalDurationNs"].IsNumber())
						{
							entry.totalDurationNs = entryJson["totalDurationNs"].GetNumber();
						}
						if (entryJson.Contains("meanNs") && entryJson["meanNs"].IsNumber())
						{
							entry.meanNs = entryJson["meanNs"].GetNumber();
						}
						if (entryJson.Contains("varianceNs") && entryJson["varianceNs"].IsNumber())
						{
							entry.varianceNs = entryJson["varianceNs"].GetNumber();
						}
						if (entryJson.Contains("stddevNs") && entryJson["stddevNs"].IsNumber())
						{
							entry.stddevNs = entryJson["stddevNs"].GetNumber();
						}
						if (entryJson.Contains("minNs") && entryJson["minNs"].IsNumber())
						{
							entry.minNs = entryJson["minNs"].GetNumber();
						}
						if (entryJson.Contains("maxNs") && entryJson["maxNs"].IsNumber())
						{
							entry.maxNs = entryJson["maxNs"].GetNumber();
						}
						if (entryJson.Contains("iterationsPerSecond") && entryJson["iterationsPerSecond"].IsNumber())
						{
							entry.iterationsPerSecond = entryJson["iterationsPerSecond"].GetNumber();
						}

						entries.push_back(std::move(entry));
					}

					history_[benchmarkName] = std::move(entries);
				}
			}
			catch (const std::exception&)
			{
				// If parsing fails, start fresh
				history_.clear();
			}

			loaded_ = true;
		}

		void SaveHistory()
		{
			const auto path = HistoryFilePath();

			// Ensure parent directory exists
			utility::EnsureDirectory(path.parent_path());

			// Write to temp file first for atomicity
			const auto tempPath = path.parent_path() / (path.filename().string() + ".tmp");

			{
				std::ofstream file(tempPath);
				if (!file)
				{
					throw std::runtime_error(std::format("Failed to open file for writing: {}", tempPath.string()));
				}

				utility::JSON json;

				for (const auto& [benchmarkName, entries]: history_)
				{
					std::vector<utility::JSON> entryArray;
					entryArray.reserve(entries.size());

					for (const auto& entry: entries)
					{
						utility::JSON entryJson;
						entryJson["timestamp"] = entry.timestamp;
						entryJson["iterations"] = static_cast<std::uint32_t>(entry.iterations);
						entryJson["samples"] = static_cast<std::uint32_t>(entry.samples);
						entryJson["totalDurationNs"] = entry.totalDurationNs;
						entryJson["meanNs"] = entry.meanNs;
						entryJson["varianceNs"] = entry.varianceNs;
						entryJson["stddevNs"] = entry.stddevNs;
						entryJson["minNs"] = entry.minNs;
						entryJson["maxNs"] = entry.maxNs;
						entryJson["iterationsPerSecond"] = entry.iterationsPerSecond;
						entryArray.push_back(std::move(entryJson));
					}

					json[benchmarkName].AssignRange(std::move(entryArray));
				}

				file << json;
			}

			// Atomic rename
			std::error_code ec;
			std::filesystem::rename(tempPath, path, ec);
			if (ec)
			{
				// Fallback: try to copy and remove
				std::filesystem::copy_file(tempPath, path, std::filesystem::copy_options::overwrite_existing, ec);
				std::filesystem::remove(tempPath, ec);
			}
		}

		static auto CurrentTimestamp() -> std::string
		{
			const auto now = std::chrono::system_clock::now();
			return std::format("{:%Y-%m-%dT%H:%M:%S}", std::chrono::floor<std::chrono::seconds>(now));
		}

		HistoryConfig config_;
		std::map<std::string, std::vector<HistoricalEntry>> history_;
		bool loaded_ = false;
	};
}
