export module synodic.honesty.trace:tracer;

import std;

import :context;
import :types;
import :span;
import zstring_view;

namespace honesty::trace
{
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
		consteval Tracer() = default;

		Tracer(const Tracer& other)                          = delete;
		constexpr Tracer(Tracer&& other) noexcept            = default;
		Tracer& operator=(const Tracer& other)               = delete;
		constexpr Tracer& operator=(Tracer&& other) noexcept = default;

		constexpr auto CreateSpan(const std::string_view label) const
		{
			if constexpr (Configuration.enabled)
			{
				return Span(label);
			}
			else
			{
				return NoopSpan();
			}
		}

		static consteval TracerConfiguration GetConfiguration()
		{
			return Configuration;
		}

	private:
		TracerOptions options_;
	};
}
