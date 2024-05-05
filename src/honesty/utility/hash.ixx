export module synodic.honesty.utility:hash;

import std;

namespace synodic::honesty::utility
{

	export template<typename T>
	consteval auto Hash(const T& value)
	{
		return std::hash<T>{}(value);
	}
}
