export module synodic.honesty.trace:tracer;

import std;

import :context;
import :span;
import zstring_view;

namespace synodic::honesty::trace
{
	/**
	 * @summary A tracer generates and provides Trace objects.
	 */
	export class Tracer
	{
	public:

		static consteval Tracer& Get(std::zstring_view label)
		{
			// TODO: Replace with a retrieval
			return tracer_;
		}

	private:
		explicit Tracer(std::zstring_view label)
		{
		}

		static Tracer tracer_;
	};
}
