
export module synodic.honesty.test:expression;

import std;

// Private namespace
namespace synodic::honesty
{

}

export namespace synodic::honesty
{
	template<typename T>
	constexpr auto expect(const T& expression, const std::source_location& location = std::source_location::current())
	{
		return expression;
	}
}
