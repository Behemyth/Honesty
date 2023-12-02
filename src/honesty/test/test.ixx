
export module synodic.honesty.test:test;

import std;
import :generator;

export namespace synodic::honesty
{
	class Test;

	// template<std::invocable Fn>
	// auto Test(std::string_view name, Fn&& generator)
	//{
	//	return std::ranges::elements_of(generator());
	// }

	/**
	 * @brief Strongly typed definition around string_view with construction
	 */
	class [[nodiscard]] TestName
	{
	public:
		consteval TestName(std::string_view name) :
			name_(name) {

			};

		TestName(const TestName& other)				   = delete;
		TestName(TestName&& other) noexcept			   = delete;
		TestName& operator=(const TestName& other)	   = delete;
		TestName& operator=(TestName&& other) noexcept = delete;

		template<std::invocable Fn>
		auto operator=(Fn&& generator)
		{
			return Test(name_, generator);
		}

	protected:
		std::string_view name_;
	};

	class Test final
	{
	public:
		template<std::invocable Fn>
		Test(std::string_view name, Fn&& test);

		template<std::invocable Fn>
		Test& operator=(Fn&& test);
	};

	template<std::invocable Fn>
	Test::Test(std::string_view name, Fn&& test)
	{
	}

	template<std::invocable Fn>
	Test& Test::operator=(Fn&& test)
	{
		return *this;
	}

	// template<>
	// class Test<void> final
	//{
	// public:
	//	Test(std::string_view name, std::move_only_function<void()> runner);

	//	Test& operator=(std::move_only_function<void()> generator);

	// protected:
	//	void Run() override;

	// private:
	//	std::move_only_function<void()> runner_;
	// };

	// template<typename T>
	// Test<T>::Test(std::string_view name, std::move_only_function<void(const T&)> runner) :
	//	runner_(std::move(runner))
	//{
	// }

	// template<typename T>
	// Test<T>& Test<T>::operator=(std::move_only_function<void(const T&)> runner)
	//{
	//	runner_ = std::move(runner);
	//	return *this;
	// }

	// template<typename T>
	// void Test<T>::Run()
	//{
	// }

	// Template Deductions

	// Test(std::string_view, std::move_only_function<void()>) -> Test<void>;

	// template<typename T>
	// Test(std::string_view, std::move_only_function<void(const T&)>) -> Test<T>;

	// Operators

	using Generator = std::generator<Test>;

	template<std::invocable<int> Fn>
	[[nodiscard]] constexpr Generator operator|(const Fn&& test, const std::ranges::range auto& range)
	{
		for (const auto& value: range)
		{
			co_yield Test("", test);
		}
	}

	template<typename Fn, typename... Types>
		requires(std::invocable<Fn, Types> && ...)
	[[nodiscard]] constexpr Generator operator|(const Fn&& test, std::tuple<Types...>&& tuple)
	{
		co_yield Test("", test);
	}

}
