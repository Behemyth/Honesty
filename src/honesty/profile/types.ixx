export module synodic.honesty.profile:types;

import std;

namespace synodic::honesty::profile
{
	using Clock = std::conditional_t<
		std::chrono::high_resolution_clock::is_steady,
		std::chrono::high_resolution_clock,
		std::chrono::steady_clock>;

}
