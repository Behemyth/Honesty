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
				totalDuration(0),
				targetDuration(targetDuration)
			{
			}

			bool Iterate()
			{
				return --targetIterations > 0;
			}

			/**
			 *	@brief Updates the internal metric state with the sample from the last generation set
			 */
			bool Push(const Duration& duration)
			{
				const double elapsed = duration.count();
				totalDuration		+= duration;

				// Track the iteration count for this sample before updating
				lastIterationCount = targetIterations + 1;	// +1 because we decremented past 0
				totalIterations	  += lastIterationCount;

				// Set the next sample's iteration count
				// TODO: Add random variation to the iteration count to avoid aliasing
				targetIterations = static_cast<std::uint32_t>(targetDuration.count() / elapsed * lastIterationCount);

				return targetIterations > 0;
			}

			std::uint32_t targetIterations;
			std::uint32_t lastIterationCount;
			std::size_t totalIterations;

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
	};

}
