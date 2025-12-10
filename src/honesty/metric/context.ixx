export module synodic.honesty.metric:context;

import std;
import function_ref;

import :types;
import :timer;
import :results;

namespace honesty::metric
{
	/**
	 *	@brief Context for timed state. i.e. Run a measurement for a certain amount of time.
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

			return results;
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
		 * @brief Initializes the context to an estimated minimal fit of iterations/samples
		 */
		RegressionContext() :
			TimedContext(std::chrono::nanoseconds(1))
		{
			// TODO: Move the calculation to the initializer
			const Duration resolution(20);	// TODO: Get the resolution of the current clock

			// TODO: Config the multiplier
			targetSampleDuration_ = resolution * 1000;
		}

		/**
		 * @brief Measures a function and stores the results internally for later retrieval
		 * @param metric The function to measure
		 * @return The measurement results
		 */
		Results Measure(const std::function_ref<void()> metric)
		{
			lastResults_ = TimedContext::Measure(metric);
			return lastResults_;
		}

		/**
		 * @brief Returns the results from the last Measure() call
		 * @return The last measurement results
		 */
		const Results& GetResults() const
		{
			return lastResults_;
		}

	private:
		Results lastResults_;
	};

}
