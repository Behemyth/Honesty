/**
 * @file comparison.ixx
 * @brief Benchmark comparison display formatting
 *
 * Provides attractive formatting for benchmark comparison results,
 * including percentage changes, status indicators, and summary tables.
 */

export module synodic.honesty.metric:comparison;

import :history;
import :results;

import std;

namespace honesty::metric
{
	/**
	 * @brief ANSI color codes for terminal output
	 */
	namespace colors
	{
		inline constexpr std::string_view Reset = "\033[0m";
		inline constexpr std::string_view Bold = "\033[1m";
		inline constexpr std::string_view Red = "\033[31m";
		inline constexpr std::string_view Green = "\033[32m";
		inline constexpr std::string_view Yellow = "\033[33m";
		inline constexpr std::string_view Blue = "\033[34m";
		inline constexpr std::string_view Cyan = "\033[36m";
		inline constexpr std::string_view Gray = "\033[90m";
	}

	/**
	 * @brief Unicode symbols for status indication
	 */
	namespace symbols
	{
		inline constexpr std::string_view Improved = "▲"; // or "↑" or "✓"
		inline constexpr std::string_view Regressed = "▼"; // or "↓" or "✗"
		inline constexpr std::string_view Unchanged = "≈"; // or "→" or "~"
		inline constexpr std::string_view Separator = "│";
		inline constexpr std::string_view HorizontalLine = "─";
	}

	/**
	 * @brief Configuration for comparison display formatting
	 */
	export struct DisplayConfig
	{
		bool useColor = true;		// Use ANSI color codes
		bool useUnicode = true;		// Use Unicode symbols
		std::size_t nameWidth = 40; // Column width for benchmark names
		std::size_t valueWidth = 12; // Column width for numeric values
	};

	/**
	 * @brief Format a duration in human-readable form
	 */
	export auto FormatDuration(double nanoseconds) -> std::string
	{
		if (nanoseconds < 1000.0)
		{
			return std::format("{:.2f} ns", nanoseconds);
		}
		if (nanoseconds < 1'000'000.0)
		{
			return std::format("{:.2f} µs", nanoseconds / 1000.0);
		}
		if (nanoseconds < 1'000'000'000.0)
		{
			return std::format("{:.2f} ms", nanoseconds / 1'000'000.0);
		}
		return std::format("{:.2f} s", nanoseconds / 1'000'000'000.0);
	}

	/**
	 * @brief Format a throughput value
	 */
	export auto FormatThroughput(double opsPerSecond) -> std::string
	{
		if (opsPerSecond < 1000.0)
		{
			return std::format("{:.1f} ops/s", opsPerSecond);
		}
		if (opsPerSecond < 1'000'000.0)
		{
			return std::format("{:.1f} Kops/s", opsPerSecond / 1000.0);
		}
		if (opsPerSecond < 1'000'000'000.0)
		{
			return std::format("{:.1f} Mops/s", opsPerSecond / 1'000'000.0);
		}
		return std::format("{:.1f} Gops/s", opsPerSecond / 1'000'000'000.0);
	}

	/**
	 * @brief Format a percentage change with appropriate sign and color
	 */
	export auto FormatPercentChange(double change, bool lowerIsBetter, const DisplayConfig& config = {}) -> std::string
	{
		const double percent = change * 100.0;
		const bool isImprovement = lowerIsBetter ? (change < 0) : (change > 0);
		const bool isRegression = lowerIsBetter ? (change > 0) : (change < 0);

		std::string prefix;
		std::string suffix;

		if (config.useColor)
		{
			if (isImprovement)
			{
				prefix = std::string(colors::Green);
			}
			else if (isRegression)
			{
				prefix = std::string(colors::Red);
			}
			else
			{
				prefix = std::string(colors::Gray);
			}
			suffix = std::string(colors::Reset);
		}

		const char sign = (percent >= 0) ? '+' : '\0';
		if (sign)
		{
			return std::format("{}+{:.1f}%{}", prefix, percent, suffix);
		}
		return std::format("{}{:.1f}%{}", prefix, percent, suffix);
	}

	/**
	 * @brief Get the status symbol for a comparison result
	 */
	export auto GetStatusSymbol(ComparisonResult::Status status, const DisplayConfig& config = {}) -> std::string
	{
		std::string prefix;
		std::string suffix;
		std::string_view symbol;

		switch (status)
		{
			case ComparisonResult::Status::Improved:
				symbol = config.useUnicode ? symbols::Improved : "^";
				if (config.useColor)
				{
					prefix = std::string(colors::Green) + std::string(colors::Bold);
					suffix = std::string(colors::Reset);
				}
				break;
			case ComparisonResult::Status::Regressed:
				symbol = config.useUnicode ? symbols::Regressed : "v";
				if (config.useColor)
				{
					prefix = std::string(colors::Red) + std::string(colors::Bold);
					suffix = std::string(colors::Reset);
				}
				break;
			case ComparisonResult::Status::Unchanged:
			default:
				symbol = config.useUnicode ? symbols::Unchanged : "~";
				if (config.useColor)
				{
					prefix = std::string(colors::Gray);
					suffix = std::string(colors::Reset);
				}
				break;
		}

		return std::format("{}{}{}", prefix, symbol, suffix);
	}

	/**
	 * @brief Get a text description of the status
	 */
	export auto GetStatusText(ComparisonResult::Status status, const DisplayConfig& config = {}) -> std::string
	{
		std::string prefix;
		std::string suffix;

		if (config.useColor)
		{
			suffix = std::string(colors::Reset);
		}

		switch (status)
		{
			case ComparisonResult::Status::Improved:
				if (config.useColor) prefix = std::string(colors::Green);
				return std::format("{}FASTER{}", prefix, suffix);
			case ComparisonResult::Status::Regressed:
				if (config.useColor) prefix = std::string(colors::Red);
				return std::format("{}SLOWER{}", prefix, suffix);
			case ComparisonResult::Status::Unchanged:
			default:
				if (config.useColor) prefix = std::string(colors::Gray);
				return std::format("{}SAME{}", prefix, suffix);
		}
	}

	/**
	 * @brief Format a single comparison result as a detailed block
	 */
	export auto FormatComparison(const ComparisonResult& result, const DisplayConfig& config = {}) -> std::string
	{
		std::string output;

		// Header with benchmark name and status
		const auto symbol = GetStatusSymbol(result.status, config);
		const auto statusText = GetStatusText(result.status, config);

		if (config.useColor)
		{
			output += std::format("{}{}{}", colors::Bold, result.benchmarkName, colors::Reset);
		}
		else
		{
			output += result.benchmarkName;
		}
		output += std::format(" {} {}\n", symbol, statusText);

		// Timing comparison
		output += std::format("  Mean:       {} → {} ({})\n",
			FormatDuration(result.previous.meanNs),
			FormatDuration(result.current.meanNs),
			FormatPercentChange(result.meanChangePercent, true, config));

		output += std::format("  Std Dev:    {} → {} ({})\n",
			FormatDuration(result.previous.stddevNs),
			FormatDuration(result.current.stddevNs),
			FormatPercentChange(result.stddevChangePercent, true, config));

		output += std::format("  Throughput: {} → {} ({})\n",
			FormatThroughput(result.previous.iterationsPerSecond),
			FormatThroughput(result.current.iterationsPerSecond),
			FormatPercentChange(result.throughputChangePercent, false, config));

		output += std::format("  Iterations: {} samples, {} iterations\n",
			result.current.samples, result.current.iterations);

		return output;
	}

	/**
	 * @brief Format a comparison result as a single table row
	 */
	export auto FormatComparisonRow(const ComparisonResult& result, const DisplayConfig& config = {}) -> std::string
	{
		const auto symbol = GetStatusSymbol(result.status, config);
		const auto change = FormatPercentChange(result.meanChangePercent, true, config);

		// Truncate name if needed
		std::string name = result.benchmarkName;
		if (name.length() > config.nameWidth)
		{
			name = "..." + name.substr(name.length() - config.nameWidth + 3);
		}

		return std::format("{} {:<{}} {:>{}} {:>{}} {:>{}}\n",
			symbol,
			name, config.nameWidth,
			FormatDuration(result.previous.meanNs), config.valueWidth,
			FormatDuration(result.current.meanNs), config.valueWidth,
			change, config.valueWidth + 2);
	}

	/**
	 * @brief Format a table header for comparison results
	 */
	export auto FormatComparisonHeader(const DisplayConfig& config = {}) -> std::string
	{
		std::string output;

		if (config.useColor)
		{
			output += std::string(colors::Bold);
		}

		output += std::format("  {:<{}} {:>{}} {:>{}} {:>{}}\n",
			"Benchmark", config.nameWidth,
			"Previous", config.valueWidth,
			"Current", config.valueWidth,
			"Change", config.valueWidth + 2);

		if (config.useColor)
		{
			output += std::string(colors::Reset);
		}

		// Separator line
		if (config.useUnicode)
		{
			output += "  ";
			for (std::size_t i = 0; i < config.nameWidth + config.valueWidth * 3 + 8; ++i)
			{
				output += symbols::HorizontalLine;
			}
			output += "\n";
		}
		else
		{
			output += std::format("  {:-<{}}\n", "", config.nameWidth + config.valueWidth * 3 + 8);
		}

		return output;
	}

	/**
	 * @brief Format a summary of multiple comparison results
	 */
	export auto FormatComparisonSummary(
		const std::vector<ComparisonResult>& results,
		const DisplayConfig& config = {}) -> std::string
	{
		if (results.empty())
		{
			return "No benchmark comparisons available.\n";
		}

		std::string output;

		// Count by status
		std::size_t improved = 0;
		std::size_t regressed = 0;
		std::size_t unchanged = 0;

		for (const auto& result: results)
		{
			switch (result.status)
			{
				case ComparisonResult::Status::Improved:
					++improved;
					break;
				case ComparisonResult::Status::Regressed:
					++regressed;
					break;
				case ComparisonResult::Status::Unchanged:
					++unchanged;
					break;
			}
		}

		// Summary header
		if (config.useColor)
		{
			output += std::format("{}Benchmark Comparison Summary{}\n", colors::Bold, colors::Reset);
		}
		else
		{
			output += "Benchmark Comparison Summary\n";
		}

		if (config.useUnicode)
		{
			output += "════════════════════════════\n";
		}
		else
		{
			output += "============================\n";
		}

		// Status counts
		if (config.useColor)
		{
			output += std::format("  {}✓ {} improved{}", colors::Green, improved, colors::Reset);
			output += std::format("  {}✗ {} regressed{}", colors::Red, regressed, colors::Reset);
			output += std::format("  {}~ {} unchanged{}\n\n", colors::Gray, unchanged, colors::Reset);
		}
		else
		{
			output += std::format("  {} improved  {} regressed  {} unchanged\n\n", improved, regressed, unchanged);
		}

		// Table
		output += FormatComparisonHeader(config);

		for (const auto& result: results)
		{
			output += FormatComparisonRow(result, config);
		}

		return output;
	}

	/**
	 * @brief Format benchmark history trend
	 */
	export auto FormatHistoryTrend(
		const std::string& benchmarkName,
		const std::vector<HistoricalEntry>& history,
		std::size_t maxEntries = 5,
		const DisplayConfig& config = {}) -> std::string
	{
		if (history.empty())
		{
			return std::format("No history for '{}'\n", benchmarkName);
		}

		std::string output;

		if (config.useColor)
		{
			output += std::format("{}History: {}{}\n", colors::Bold, benchmarkName, colors::Reset);
		}
		else
		{
			output += std::format("History: {}\n", benchmarkName);
		}

		const std::size_t count = std::min(maxEntries, history.size());

		for (std::size_t i = 0; i < count; ++i)
		{
			const auto& entry = history[i];
			std::string changeStr;

			if (i + 1 < history.size())
			{
				const auto& prev = history[i + 1];
				if (prev.meanNs > 0)
				{
					const double change = (entry.meanNs - prev.meanNs) / prev.meanNs;
					changeStr = " " + FormatPercentChange(change, true, config);
				}
			}

			if (config.useColor && i == 0)
			{
				output += std::format("  {}→{} ", colors::Cyan, colors::Reset);
			}
			else
			{
				output += "    ";
			}

			output += std::format("{} {} {}{}\n",
				entry.timestamp,
				FormatDuration(entry.meanNs),
				FormatThroughput(entry.iterationsPerSecond),
				changeStr);
		}

		if (history.size() > maxEntries)
		{
			if (config.useColor)
			{
				output += std::format("  {}... and {} more entries{}\n",
					colors::Gray, history.size() - maxEntries, colors::Reset);
			}
			else
			{
				output += std::format("  ... and {} more entries\n", history.size() - maxEntries);
			}
		}

		return output;
	}
}
