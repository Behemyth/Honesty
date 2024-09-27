/**
 *	@brief Type traits
 */

export module synodic.honesty.utility:traits;

import std;

namespace synodic::honesty::utility
{
	/**
	 *	Evaluates the given integer and returns the minimally sized integral type that can hold the value. If the value
	 *		cannot be held by any integral type the alias will resolve to void.
	 */
	export template<std::size_t N>
	using MinimalIntegerType = std::conditional_t<
		N <= std::numeric_limits<std::uint8_t>::max(),
		std::uint8_t,
		std::conditional_t<
			N <= std::numeric_limits<std::uint16_t>::max(),
			std::uint16_t,
			std::conditional_t<
				N <= std::numeric_limits<std::uint32_t>::max(),
				std::uint32_t,
				std::conditional_t<
					N <= std::numeric_limits<std::uint64_t>::max(),
					std::uint64_t,
					std::conditional_t<N <= std::numeric_limits<std::uintmax_t>::max(), std::uintmax_t, void>>>>>;
}
