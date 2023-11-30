export module synodic.honesty.test:literals;

import :test;
import :suite;
import :generator;
import :set;

export namespace synodic::honesty::literals
{
	[[nodiscard]] consteval auto operator""_test(const char* const name, const std::size_t size)
	{
		return TestName(std::string_view(name, size));
	}

	[[nodiscard]] consteval auto operator""_set(const char* const name, const std::size_t size)
	{
		return SetName(std::string_view(name, size));
	}
}
