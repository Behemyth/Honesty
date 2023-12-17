export module synodic.honesty.test:literals;

import :test;
import :generator;
import :tag;

export namespace synodic::honesty::literals
{
	[[nodiscard]] consteval auto operator""_test(const char* const name, const std::size_t size)
	{
		return TestName(std::string_view(name, size));
	}

	[[nodiscard]] consteval auto operator""_tag(const char* const name, const std::size_t size)
	{
		return tag(std::string_view(name, size));
	}
}
