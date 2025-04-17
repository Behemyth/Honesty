export module synodic.honesty.metric:timer;

import :types;
import std;

namespace synodic::honesty::metric
{

	/**
	 *	@brief A collection of durations that may optionally exist.
	 */
	class Durations
	{
	public:

	private:
	};

	export class Timer
	{
	public:
		explicit Timer(Duration& duration) :
			start_(Clock::now()),
			duration_(duration)
		{
		}

		~Timer()
		{
			duration_.get() = Clock::now() - start_;
		}

	private:
		Clock::time_point start_;
		std::reference_wrapper<Duration> duration_;
	};
}
