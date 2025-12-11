export module synodic.honesty.trace:scope;

import std;
import zstring_view;

import :types;

namespace honesty::trace
{
	/**
	 * @brief Compile-time metadata for a trace span. Used to define span properties at compile time.
	 */
	export class Scope final
	{
	public:
		explicit consteval Scope(std::zstring_view label, SpanKind kind = SpanKind::INTERNAL) noexcept :
			label_(label),
			kind_(kind)
		{
		}

		Scope(const Scope&) = delete;
		Scope& operator=(const Scope&) = delete;

		Scope(Scope&&) noexcept = default;
		Scope& operator=(Scope&&) noexcept = default;

		consteval std::zstring_view Label() const noexcept { return label_; }
		consteval SpanKind Kind() const noexcept { return kind_; }

	private:
		std::zstring_view label_;
		SpanKind kind_;
	};
}
