export module synodic.honesty.metric:types;

import std;

namespace honesty::metric
{
	export using Clock = std::chrono::steady_clock;
	export using Duration = Clock::duration;
}
