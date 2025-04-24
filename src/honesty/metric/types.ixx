export module synodic.honesty.metric:types;

import std;

namespace honesty::metric
{
	export using Clock = std::conditional_t<
		std::chrono::high_resolution_clock::is_steady,
		std::chrono::high_resolution_clock,
		std::chrono::steady_clock>;

	export using Duration = Clock::duration;

}
