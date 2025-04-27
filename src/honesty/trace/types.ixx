export module synodic.honesty.trace:types;

import std;

namespace honesty::trace
{
	export template<typename Enum>
	concept group_enum =
		std::is_scoped_enum_v<Enum>
		&& requires
		{
			{ Enum::COUNT } -> std::convertible_to<Enum>; // TODO: Replace with reflection, C++26
		};
}
