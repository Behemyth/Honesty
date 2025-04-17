export module synodic.honesty.trace:zone;

import std;
import zstring_view;

namespace synodic::honesty::trace
{
	/**
	 * @brief Metadata for a trace span
	 */
	export class Zone final
    {
    public:

        explicit consteval Zone(std::zstring_view label) noexcept
        {
        }

        Zone(const Zone&) = delete;
        Zone& operator=(const Zone&) = delete;

        Zone(Zone&&) noexcept = default;
        Zone& operator=(Zone&&) noexcept = default;

    private:

    };
}
