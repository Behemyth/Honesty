export module synodic.honesty.trace:tracer;

import std;

import :context;
import :types;
import zstring_view;

namespace honesty::trace
{
	export struct TracerConfiguration
	{
		explicit constexpr TracerConfiguration(const bool enabled) :
			enabled(enabled)
		{
		}

		bool enabled;
	};

	/**
	 * @brief A tracer is a tag for Trace objects.
	 */
	export
	class Tracer
	{
	public:
		constexpr Tracer(const Tracer& other)                = delete;
		constexpr Tracer(Tracer&& other) noexcept            = default;
		constexpr Tracer& operator=(const Tracer& other)     = delete;
		constexpr Tracer& operator=(Tracer&& other) noexcept = default;

	private:
		template<group_enum T>
		friend class ProviderBuilder;

		constexpr explicit Tracer(const TracerConfiguration& configuration)
		{
		}
	};
}
