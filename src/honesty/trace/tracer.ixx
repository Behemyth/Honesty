export module synodic.honesty.trace:tracer;

import std;

import :context;
import :types;
import zstring_view;

namespace honesty::trace
{
	struct TracerConfiguration
	{
		bool enabled;
	};

	/**
	 * @brief A tracer is a tag for Trace objects.
	 */
	export
	class Tracer
	{
	public:
		Tracer(const Tracer& other)                = delete;
		Tracer(Tracer&& other) noexcept            = delete;
		Tracer& operator=(const Tracer& other)     = delete;
		Tracer& operator=(Tracer&& other) noexcept = delete;
	private:
		template<group_enum T>
		friend class ProviderBuilder;

		consteval explicit Tracer(const TracerConfiguration& configuration)
		{
		}
	};
}
