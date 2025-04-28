export module synodic.honesty.trace:tracer;

import std;

import :context;
import :types;
import zstring_view;

namespace honesty::trace
{
	/**
	 * @brief A tracer is a tag for Trace objects.
	 */
	export
	class Tracer
	{
	public:


	private:
		template<group_enum T>
		friend class Provider;

		consteval explicit Tracer()
		{
		}
	};
}
