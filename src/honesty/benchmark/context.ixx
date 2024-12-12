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
			State state;

			while (state.Iterate())
			{
				Duration duration;

				// Scope the timer to just the sampled data
				{
					Timer timer(duration);

					benchmark();
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
			State() :
				totalDuration(0),
				totalIterations(0)
			{
			}

			/**
			 *	@brief Marks that an iteration of executing the benchmark should start. Returns true if benchmark
			 *		iteration should continue
			 */
			bool Iterate()
			{
			}

			/**
			 *	@brief Updates the internal benchmark state with the sample from the last generation set
			 */
			void Update(const Duration& duration)
			{
			}

			std::chrono::nanoseconds totalDuration;
			std::uint32_t totalIterations;
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

		RegressionContext() :
			TimedContext(std::chrono::nanoseconds(1))
		{
			// TODO: Move the calculation to the initializer
			const Duration resolution(20);	// TODO: Get the resolution of the current clock

			// TODO: Config the multiplier
			targetSampleDuration_ = resolution * 1000;
		}

		Results Measure(const std::function_ref<void()> benchmark)
		{
			// TODO: Minimize wrapping logic around the executed function
			State state;

			// Each measurement generation depends on the previous generation's duration. We spend the minimal time
			//	possible to retrieve an accurate benchmark and only collect multiple samples if necessary
			while (state.Generate())
			{
				// All iterations are measured under a single duration. Avoids the overhead of measuring each iteration
				Duration duration;

				// Scope the timer to just the sampled data
				{
					Timer timer(duration);

					while (state.Iterate())
					{
						benchmark();
					}
				}

				state.Update(duration);
			}

			Results results;

			return results;
		}

	private:
		/**
		 *	@brief Data that exists for the lifetime of a measurement
		 */
		struct State : TimedContext::State
		{
			/**
			 *	@brief Generates the next set of iterations for the benchmark. Returns true if the generation was
			 *initiated
			 */
			bool Generate()
			{
				currentIteration_ = currentIterationCount_;
				return currentIterationCount_ != 0;
			}

			void Update(const Duration& duration)
			{
				TimedContext::State::Update(duration);

				// Set the next generation iteration count
				// TODO: Add random variation to the iteration count to avoid aliasing
				currentIterationCount_ = targetSampleDuration_ / duration * currentIterationCount_;
			}
		};
	};

}
