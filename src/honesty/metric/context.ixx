export module synodic.honesty.metric:context;

import std;
import function_ref;

import :types;
import :timer;
import :results;
import :platform;
import synodic.honesty.log;

namespace honesty::metric
{
	namespace
	{
		auto& MetricContextLogger()
		{
			static log::Logger logger = []() {
				log::Logger l = log::RootLogger().CreateLogger("honesty.metric.context");
				l.SetLevel(log::LevelType::WARNING);
				return l;
			}();
			return logger;
		}
	}

	/**
	 *	@brief Context for timed state. i.e. Run a measurement for a certain amount of time.
	 *
	 *	When MetricsEnabled is false (via -DHONESTY_METRICS_DISABLED), the Measure method
	 *	compiles to just executing the metric function once and returning empty results.
	 */
	export class TimedContext
	{
	public:
		explicit(false) TimedContext(const std::chrono::nanoseconds duration) :
			targetSampleDuration_(duration)
		{
		}

		Results Measure(const std::function_ref<void()> metric)
		{
			if constexpr (!MetricsEnabled)
			{
				// When metrics are disabled, just run the function once (for side effects)
				// and return empty results. The optimizer will inline this to nearly nothing.
				metric();
				return Results{};
			}
			else
			{
				MetricContextLogger().Debug("Starting timed measurement (target: {}ns)",
					targetSampleDuration_.count());

				// TODO: Minimize wrapping logic around the executed function
				State state(targetSampleDuration_);
				BenchmarkAccumulator accumulator;

				// Each measurement depends on the previous sample.
				while (true)
				{
					// All iterations are measured under a single sample. Avoids the overhead of measuring each iteration
					Duration duration;

					// Scope the timer to just the executed operation
					{
						Timer timer(duration);

						while (state.Iterate())
						{
							metric();
						}
					}

					// Push normalized per-iteration time to accumulator
					if (state.lastIterationCount > 0)
					{
						const double iterationTimeNs =
							static_cast<double>(duration.count()) / static_cast<double>(state.lastIterationCount);
						accumulator.Push(iterationTimeNs);
					}

					if (not state.Push(duration))
					{
						break;
					}
				}

				Results results;
				results.FromAccumulator(accumulator, state.totalIterations, state.totalDuration);

				MetricContextLogger().Trace("Measurement complete: {} iterations, {} samples, mean={}ns",
					results.iterations, results.samples, results.mean.count());

				return results;
			}
		}

	protected:
		/**
		 *	@brief Data that exists for the lifetime of a measurement
		 */
		struct State
		{
			explicit State(const std::chrono::nanoseconds targetDuration) :
				targetIterations(1),
				lastIterationCount(0),
				totalIterations(0),
				sampleCount(0),
				totalDuration(0),
				targetDuration(targetDuration)
			{
			}

			bool Iterate()
			{
				if (targetIterations > 0)
				{
					--targetIterations;
					++lastIterationCount;
					return true;
				}
				return false;
			}

			/**
			 *	@brief Updates the internal metric state with the sample from the last generation set
			 *	@return true if more samples should be collected, false if measurement is complete
			 */
			bool Push(const Duration& duration)
			{
				const double elapsed = duration.count();
				totalDuration		+= duration;

				// lastIterationCount is updated during Iterate()
				totalIterations += lastIterationCount;
				++sampleCount;

				// Check if we've collected enough samples or exceeded time budget
				// Minimum 1 sample, stop after reasonable sample count or time
				if (sampleCount >= maxSamples || totalDuration >= targetDuration)
				{
					return false;
				}

				// Set the next sample's iteration count based on timing
				std::uint32_t nextIterations = 1;
				if (elapsed > 0)
				{
					// Scale iterations to try to hit target duration per sample
					const double scale = static_cast<double>(targetDuration.count()) / elapsed;
					nextIterations = std::max(1u, static_cast<std::uint32_t>(scale * lastIterationCount));
				}

				// Reset for next sample
				lastIterationCount = 0;
				targetIterations = nextIterations;

				return true;
			}

			std::uint32_t targetIterations;
			std::uint32_t lastIterationCount;
			std::size_t totalIterations;
			std::size_t sampleCount;
			static constexpr std::size_t maxSamples = 100;

			std::chrono::nanoseconds totalDuration;
			std::chrono::nanoseconds targetDuration;
		};

		std::chrono::nanoseconds targetSampleDuration_;
	};

	/**
	 *	@brief Context for iterative state. i.e. Run a measurement for a certain number of iterations. Because the real
	 *		resolution of the underlying hardware clock isn't known we internally introduce a timed context to provide
	 *		enough variability to provide meaningful results, scaling the iterations accordingly.
	 */
	export class RegressionContext : public TimedContext
	{
	public:
		/**
		 * @brief Configuration for regression measurement
		 */
		struct Config
		{
			/// Number of warmup iterations before measurement (default: 0)
			std::uint64_t warmupIterations = 0;

			/// Platform tuning configuration
			PlatformConfig platform = {};

			/// Whether to apply platform tuning during measurement
			bool enablePlatformTuning = true;
		};

		/**
		 * @brief Initializes the context to an estimated minimal fit of iterations/samples
		 */
		explicit RegressionContext(Config config = {}) :
			TimedContext(std::chrono::nanoseconds(1)),
			config_(std::move(config))
		{
			// TODO: Move the calculation to the initializer
			const Duration resolution(20);	// TODO: Get the resolution of the current clock

			// TODO: Config the multiplier
			targetSampleDuration_ = resolution * 1000;
		}

		/**
		 * @brief Measures a function and stores the results internally for later retrieval.
		 *
		 * When MetricsEnabled is false, this compiles to just executing the metric function
		 * once and returning empty results.
		 *
		 * @param metric The function to measure
		 * @return The measurement results
		 */
		Results Measure(const std::function_ref<void()> metric)
		{
			if constexpr (!MetricsEnabled)
			{
				// When metrics are disabled, just run the function once and return empty results
				metric();
				lastResults_ = Results{};
				return lastResults_;
			}
			else
			{
				MetricContextLogger().Debug("Starting regression measurement");

				// Apply platform tuning if enabled
				std::optional<ScopedPlatformTuner> platformTuner;
				if (config_.enablePlatformTuning)
				{
					platformTuner.emplace(config_.platform);

					// Log any warnings from platform setup
					for (const auto& warning : platformTuner->State().warnings)
					{
						MetricContextLogger().Warning("Platform: {}", warning);
					}
				}

				// Run warmup iterations (not timed)
				if (config_.warmupIterations > 0)
				{
					MetricContextLogger().Debug("Running {} warmup iterations", config_.warmupIterations);
					for (std::uint64_t i = 0; i < config_.warmupIterations; ++i)
					{
						metric();
					}
				}

				// Perform actual measurement
				lastResults_ = TimedContext::Measure(metric);

				MetricContextLogger().Debug("Regression measurement complete");
				return lastResults_;
			}
		}

		/**
		 * @brief Returns the results from the last Measure() call
		 * @return The last measurement results
		 */
		const Results& GetResults() const
		{
			return lastResults_;
		}

		/**
		 * @brief Get the current configuration
		 */
		const Config& GetConfig() const noexcept
		{
			return config_;
		}

		/**
		 * @brief Set the number of warmup iterations
		 * @param count Number of iterations to run before measurement
		 * @return Reference to this context for chaining
		 */
		RegressionContext& Warmup(std::uint64_t count) noexcept
		{
			config_.warmupIterations = count;
			return *this;
		}

		/**
		 * @brief Enable or disable platform tuning
		 * @param enable Whether to apply platform-specific optimizations
		 * @return Reference to this context for chaining
		 */
		RegressionContext& EnablePlatformTuning(bool enable) noexcept
		{
			config_.enablePlatformTuning = enable;
			return *this;
		}

	private:
		Config config_;
		Results lastResults_;
	};

}
