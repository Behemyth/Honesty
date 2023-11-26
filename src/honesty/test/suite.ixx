export module synodic.honesty.test:suite;

import std;

import :generator;
import :test;

export namespace synodic::honesty
{
	class [[nodiscard]] Suite
	{
	public:
		consteval Suite(std::string_view name);

		template<std::invocable Fn>
		consteval Suite(std::string_view name, Fn&& generator);

		Suite(const Suite& other)	  = delete;
		Suite(Suite&& other) noexcept = default;

		Suite& operator=(const Suite& other)	 = delete;
		Suite& operator=(Suite&& other) noexcept = default;

		template<std::invocable Fn>
		consteval Suite& operator=(Fn&& generator) noexcept;

	protected:
		std::string_view name_;
	};

	consteval Suite::Suite(std::string_view name) :
		name_(name)
	{
	}

	template<std::invocable Fn>
	consteval Suite::Suite(std::string_view name, Fn&& generator) :
		name_(name)
	{
	}

	template<std::invocable Fn>
	consteval Suite& Suite::operator=(Fn&& generator) noexcept
	{
		return *this;
	}
}
