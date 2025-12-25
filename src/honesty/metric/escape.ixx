/** @brief Utilities to prevent compiler optimizations in benchmarks */

export module synodic.honesty.metric:escape;

import std;

namespace honesty::metric
{
	namespace detail
	{
		// Use function-local statics to avoid module linkage issues with inline variables.
		// The volatile function pointer pattern prevents the compiler from reasoning about
		// what these functions do, forcing it to preserve the calls.
		inline auto& SinkFn() noexcept
		{
			static void (*volatile fn)(void const*) noexcept = [](void const*) noexcept {};
			return fn;
		}

		inline auto& ClobberFn() noexcept
		{
			static void (*volatile fn)(void*) noexcept = [](void*) noexcept {};
			return fn;
		}
	}

	/**
	 * @brief Prevents dead code elimination of a computed value
	 * @tparam T Type of value to preserve
	 * @param value The computed value that must not be optimized away
	 */
	export template<typename T>
	void DoNotOptimize(T const& value) noexcept
	{
		detail::SinkFn()(std::addressof(value));
		std::atomic_signal_fence(std::memory_order_acq_rel);
	}

	/**
	 * @brief Prevents constant propagation and forces value to be re-read
	 * @tparam T Type of value to clobber
	 * @param value The value that must be treated as potentially modified
	 */
	export template<typename T>
	void Clobber(T& value) noexcept
	{
		detail::ClobberFn()(std::addressof(value));
		std::atomic_signal_fence(std::memory_order_acq_rel);
	}
}
