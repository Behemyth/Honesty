export module synodic.honesty.trace:trace;

import :span;
import :types;

namespace honesty::trace
{
	/**
	 * @brief A top-level span that represents the root of a trace.
	 * This is a convenience alias - any span without a parent is effectively a trace root.
	 */
	export using Trace = Span;
}
