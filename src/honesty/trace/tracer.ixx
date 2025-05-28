export module synodic.honesty.trace:tracer;

import std;

import :context;
import :types;
import :span;
import zstring_view;

namespace honesty::trace
{
	export struct TracerConfiguration
	{
		explicit consteval TracerConfiguration(const bool enabled) :
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
		consteval Tracer(const Tracer& other)                = delete;
		consteval Tracer(Tracer&& other) noexcept            = default;
		consteval Tracer& operator=(const Tracer& other)     = delete;
		consteval Tracer& operator=(Tracer&& other) noexcept = default;

		constexpr const TracerConfiguration& GetConfiguration() const
		{
			return configuration_;
		}

		consteval auto Span(const std::string_view label) const
		{
			if constexpr (configuration_.enabled)
			{
				return Span(label);
			}

			return Span(label);
		}

	private:
		template<group_enum T>
		friend class ProviderBuilder;

		consteval explicit Tracer(const TracerConfiguration& configuration) :
			configuration_(configuration)
		{
		}


		TracerConfiguration configuration_;
	};
}
