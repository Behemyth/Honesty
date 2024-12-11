export module synodic.honesty.benchmark:context;

import std;
import function_ref;

import :types;
import :timer;

namespace synodic::honesty::benchmark
{

	class State
	{
	public:
		State() :
			totalDuration_(0),
			totalIteration_(0),
			currentIteration_(0),
			currentIterationCount_(0)
		{
			const Duration resolution(20);	// TODO: Get the resolution of the current clock

			// TODO: Config the multiplier
			targetSampleDuration_ = resolution * 1000;
		}

		/**
		 *	@brief Generates the next set of iterations for the benchmark. Returns true if the generation was initiated
		 */
		bool Generate()
		{
			currentIteration_ = currentIterationCount_;
			return currentIterationCount_ != 0;
		}

		/**
		 *	@brief Marks that an iteration of executing the benchmark should start. Returns true if benchmark iteration
		 *		should continue
		 */
		bool Iterate()
		{
			return currentIteration_-- != 0;
		}

		/**
		 *	@brief Updates the internal benchmark state with the sample from the last generation set
		 */
		void Update(const Duration& duration)
		{
			totalDuration_	+= duration;
			totalIteration_ += currentIterationCount_;

			// Set the next generation iteration count
			// TODO: Add random variation to the iteration count to avoid aliasing
			currentIterationCount_ = targetSampleDuration_ / duration * currentIterationCount_;
		}

	private:
		std::chrono::nanoseconds targetSampleDuration_;

		std::chrono::nanoseconds totalDuration_;
		std::uint32_t totalIteration_;

		std::uint32_t currentIteration_;
		std::uint32_t currentIterationCount_;
	};

	/**
	 *	@brief Base class for all internal benchmarking state
	 */
	class Context
	{
	};

	/**
	 *	@brief Context for timed state. i.e. Run *this* for a certain amount of time
	 */
	export class TimedContext : public Context
	{
	public:

	private:
	};

	/**
	 *	@brief Context for iterative state. i.e. Run *this* for a certain number of iterations. Because the real
	 *		resolution of the underlying hardware clock isn't known we internally introduce a timed context to provide
	 *		enough variability to provide meaningful results
	 */
	export class RegressionContext : public TimedContext
	{
	public:
		void Run(const std::function_ref<void()> benchmark)
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
		}

	private:
	};

}
