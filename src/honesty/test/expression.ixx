
export module synodic.honesty.test:expression;

import std;

// Private namespace
namespace synodic::honesty
{

}

export namespace synodic::honesty
{

	constexpr auto expect(const bool expression, const std::source_location& location = std::source_location::current())
	{
		return expression;
	}

	template<std::convertible_to<bool> T>
	constexpr auto expect(const T& expression, const std::source_location& location = std::source_location::current())
	{
		return static_cast<bool>(expression);
	}

	template<typename T, std::derived_from<std::exception> Exception = std::exception>
	constexpr auto expect_throw(
		const T& expression,
		const std::source_location& location = std::source_location::current())
	{
		return expression;
	}
}
