export module synodic.honesty.trace:tracer;

import std;

import :context;
import :types;
import :span;
import zstring_view;

namespace honesty::trace
{
	/**
	 * @brief Compile-time options for a tracer.
	 */
	export struct TracerConfiguration
	{
		explicit consteval TracerConfiguration(const bool enabled) :
			enabled(enabled)
		{
		}

		bool enabled;
	};

	/**
	 * @brief Run-time options for a tracer.
	 */
	export struct TracerOptions
	{
		constexpr TracerOptions() = default;
	};

	/**
	 * @brief A tracer generates and manages spans.
	 */
	export
	template<TracerConfiguration Configuration>
	class Tracer
	{
	public:
		constexpr Tracer() = default;

		consteval Tracer(const Tracer& other)                = delete;
		constexpr Tracer(Tracer&& other) noexcept            = default;
		consteval Tracer& operator=(const Tracer& other)     = delete;
		constexpr Tracer& operator=(Tracer&& other) noexcept = default;

		constexpr Span CreateSpan(const std::string_view label) const
		{
			return Span(label);
		}

		constexpr TracerConfiguration GetConfiguration() const
		{
			return Configuration;
		}

	private:
		TracerOptions options_;
	};
}
