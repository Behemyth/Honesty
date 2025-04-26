export module synodic.honesty.trace:tracer;

import std;

import :context;
import zstring_view;

namespace honesty::trace
{

	/**
	 * @brief A tracer is a tag for Trace objects.
	 */
	export class Tracer
	{
	public:
		consteval std::zstring_view Label() const
		{
			return label_;
		}

	private:
		friend class Provider;

		consteval explicit Tracer(const std::zstring_view label) :
			label_(label)
		{
		}

		std::zstring_view label_;
	};
}
