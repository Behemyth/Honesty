export module synodic.honesty.test:suite;

import std;

import :generator;
import :test;

export namespace synodic::honesty
{
	class [[nodiscard]] Suite
	{
	public:
		template<std::invocable Fn>
		consteval Suite(std::string_view name, Fn&& generator);

		Suite(const Suite& other)	  = delete;
		Suite(Suite&& other) noexcept = delete;

		Suite& operator=(const Suite& other)	 = delete;
		Suite& operator=(Suite&& other) noexcept = delete;

		template<std::invocable Fn>
		consteval Suite& operator=(Fn&& generator) noexcept;

	protected:
		std::string_view name_;
	};

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
