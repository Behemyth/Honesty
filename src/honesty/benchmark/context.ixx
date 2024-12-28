export module synodic.honesty.benchmark:context;

import std;
import function_ref;

import :types;
import :timer;

namespace synodic::honesty::benchmark
{
	/**
	 *	@brief Context for timed state. i.e. Run a measurement for a certain amount of time.
	 */
	export class TimedContext
	{
	public:
		struct Results
		{
		};

		explicit(false) TimedContext(const std::chrono::nanoseconds duration) :
			targetSampleDuration_(duration)
		{
		}

		Results Measure(const std::function_ref<void()> benchmark)
		{
			// TODO: Minimize wrapping logic around the executed function
			State state(targetSampleDuration_);

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
						benchmark();
					}
				}

				if (not state.Push(duration))
				{
					break;
				}
			}

			Results results;

			return results;
		}

	protected:
		/**
		 *	@brief Data that exists for the lifetime of a measurement
		 */
		struct State
		{
			explicit State(std::chrono::nanoseconds targetDuration) :
				targetIterations(1),
				totalDuration(0)
			{
			}

			bool Iterate()
			{
				return targetIterations-- > 0;
			}

			/**
			 *	@brief Updates the internal benchmark state with the sample from the last generation set
			 */
			bool Push(const Duration& duration)
			{
				totalDuration += duration;

				// Set the next sample's iteration count
				// TODO: Add random variation to the iteration count to avoid aliasing
				targetIterations = targetDuration / duration * targetIterations;

				return targetIterations > 0;
			}

			std::uint32_t targetIterations;

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
		struct Results : TimedContext::Results
		{
		};

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
