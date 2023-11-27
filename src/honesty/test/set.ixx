export module synodic.honesty.test:set;

import std;

import :generator;
import :test;

export namespace synodic::honesty
{
	class [[nodiscard]] Set
	{
	public:
		consteval Set(std::string_view name);

		template<std::invocable Fn>
		consteval Set(std::string_view name, Fn&& generator);

		Set(const Set& other)	  = delete;
		Set(Set&& other) noexcept = default;

		Set& operator=(const Set& other)	 = delete;
		Set& operator=(Set&& other) noexcept = default;

		template<std::invocable Fn>
		consteval Set& operator=(Fn&& generator) noexcept;

		explicit operator Generator();

	protected:
		std::string_view name_;
	};

	Set::operator Generator()
	{
		co_return;
	}

	template<std::invocable Fn>
	consteval Set::Set(std::string_view name, Fn&& generator)
	{
	}

	template<std::invocable Fn>
	consteval Set& Set::operator=(Fn&& generator) noexcept
	{
		return *this;
	}

	consteval Set::Set(std::string_view name)
	{
	}
}
