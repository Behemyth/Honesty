export module synodic.honesty.trace:tracer;

import std;

import :context;
import :span;
import zstring_view;

namespace honesty::trace
{

	/**
	 * @brief A tracer generates and provides Trace objects.
	 */
	export class Tracer
	{
	public:
		consteval std::zstring_view Label() const
		{
			return label_;
		}

	private:
		template<std::uint8_t N>
		friend class Provider;

		consteval explicit Tracer(const std::zstring_view label) :
			label_(label)
		{
		}

		std::zstring_view label_;
	};
}
