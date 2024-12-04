export module synodic.honesty.profile:types;

import std;

namespace synodic::honesty::profile
{
	export using Clock = std::conditional_t<
		std::chrono::high_resolution_clock::is_steady,
		std::chrono::high_resolution_clock,
		std::chrono::steady_clock>;

	// A duration that can optionally exist
	export using Duration = std::optional<Clock::duration>;

}
