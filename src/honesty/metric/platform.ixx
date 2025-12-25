/**
 * @file platform.ixx
 * @brief Platform abstraction for benchmark stability optimizations
 *
 * Provides an interface for platform-specific tuning that reduces benchmark variance.
 * The actual implementation is provided by platform-specific modules that are
 * conditionally compiled via CMake.
 */

export module synodic.honesty.metric:platform;

import std;
import synodic.honesty.log;

namespace honesty::metric
{
	/**
	 * @brief Configuration for platform-specific benchmark tuning
	 */
	export struct PlatformConfig
	{
		/// Number of warmup iterations before measurement (default: 0)
		std::uint64_t warmupIterations = 0;

		/// Whether to attempt to set high process priority
		bool elevateProcessPriority = true;

		/// Whether to attempt to pin to a single CPU core
		bool pinToCpu = true;

		/// Whether to increase timer resolution (Windows-specific)
		bool increaseTimerResolution = true;
	};

	/**
	 * @brief Result of platform tuning operations
	 */
	export struct PlatformState
	{
		/// Whether tuning was successfully applied
		bool tuningApplied = false;

		/// Warnings generated during tuning
		std::vector<std::string> warnings;

		/// Recommendations for improving stability
		std::vector<std::string> recommendations;
	};

	/**
	 * @brief Interface for platform-specific benchmark tuning
	 *
	 * This class provides a consistent interface across platforms for
	 * optimizing benchmark stability. Platform-specific implementations
	 * are compiled conditionally via CMake.
	 *
	 * The tuner is designed to be used in a scoped manner:
	 * - Setup() is called before measurements begin
	 * - Teardown() restores original system state
	 *
	 * Thread Safety: Each thread should have its own PlatformTuner instance
	 * when running benchmarks in parallel. The tuner stores per-thread state
	 * that must be restored on teardown.
	 */
	export class PlatformTuner
	{
	public:
		explicit PlatformTuner(PlatformConfig config = {});
		~PlatformTuner();

		// Non-copyable, movable for async/threaded usage
		PlatformTuner(const PlatformTuner&) = delete;
		PlatformTuner& operator=(const PlatformTuner&) = delete;
		PlatformTuner(PlatformTuner&&) noexcept;
		PlatformTuner& operator=(PlatformTuner&&) noexcept;

		/**
		 * @brief Apply platform-specific tuning for benchmark stability
		 *
		 * This may include:
		 * - Elevating process/thread priority
		 * - Pinning to a specific CPU core
		 * - Increasing timer resolution
		 * - Other platform-specific optimizations
		 *
		 * @return State information including any warnings or recommendations
		 */
		PlatformState Setup();

		/**
		 * @brief Restore original system state
		 *
		 * Undoes any changes made by Setup(). Safe to call multiple times.
		 */
		void Teardown();

		/**
		 * @brief Check if Setup() has been called and not yet torn down
		 */
		[[nodiscard]] bool IsActive() const noexcept;

		/**
		 * @brief Get the current configuration
		 */
		[[nodiscard]] const PlatformConfig& Config() const noexcept;

	private:
		struct Impl;
		std::unique_ptr<Impl> impl_;
	};

	/**
	 * @brief RAII wrapper for automatic platform tuning
	 *
	 * Use this when you want platform tuning to be automatically
	 * set up and torn down within a scope.
	 *
	 * Example:
	 * @code
	 * {
	 *     ScopedPlatformTuner tuner;
	 *     // Benchmarks run here with tuning applied
	 * } // Tuning automatically removed
	 * @endcode
	 */
	export class ScopedPlatformTuner
	{
	public:
		explicit ScopedPlatformTuner(PlatformConfig config = {});
		~ScopedPlatformTuner();

		// Non-copyable, non-movable (scoped lifetime)
		ScopedPlatformTuner(const ScopedPlatformTuner&) = delete;
		ScopedPlatformTuner& operator=(const ScopedPlatformTuner&) = delete;
		ScopedPlatformTuner(ScopedPlatformTuner&&) = delete;
		ScopedPlatformTuner& operator=(ScopedPlatformTuner&&) = delete;

		/**
		 * @brief Get the state from Setup()
		 */
		[[nodiscard]] const PlatformState& State() const noexcept;

		/**
		 * @brief Get the underlying tuner
		 */
		[[nodiscard]] PlatformTuner& Tuner() noexcept;

	private:
		PlatformTuner tuner_;
		PlatformState state_;
	};
}

