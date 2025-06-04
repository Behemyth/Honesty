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
		
	};

	/**
	 * @brief A tracer generates and manages spans.
	 */
	export
	template<typename Configuration>
		requires std::is_same_v<Configuration, TracerConfiguration>
	class Tracer
	{
	public:
		consteval Tracer(const Tracer& other)                = delete;
		consteval Tracer(Tracer&& other) noexcept            = default;
		consteval Tracer& operator=(const Tracer& other)     = delete;
		consteval Tracer& operator=(Tracer&& other) noexcept = default;

		constexpr Span Span(const std::string_view label) const
		{
			return Span(label);
		}

	private:
		template<group_enum T>
		friend class ProviderBuilder;

		consteval explicit Tracer()
		{
		}


		TracerOptions options_;
	};
}
