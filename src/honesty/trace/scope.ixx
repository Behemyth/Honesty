export module synodic.honesty.trace:scope;

import std;
import zstring_view;

namespace honesty::trace
{
	/**
	 * @brief Metadata for a trace span
	 */
	export class Scope final
    {
    public:

        explicit consteval Scope(std::zstring_view label) noexcept
        {
        }

        Scope(const Scope&) = delete;
        Scope& operator=(const Scope&) = delete;

        Scope(Scope&&) noexcept = default;
        Scope& operator=(Scope&&) noexcept = default;

    private:

    };
}
