
export module synodic.honesty.test:test;

import std;
import :generator;
import :implementation;

namespace synodic::honesty
{

	class TestStub
	{
	};

}

export namespace synodic::honesty
{

	using Generator = generator<BaseTest>;

	template<typename T>
	class Test final : public BaseTest
	{
	public:
		Test(std::string_view name, std::move_only_function<void(const T&)> runner);

		Test& operator=(std::move_only_function<void(const T&)> runner);

	protected:
		void Run() override;

		std::move_only_function<void(const T&)> runner_;
	};

	template<>
	class Test<void> final : public BaseTest
	{
	public:
		Test(std::string_view name, std::move_only_function<void()> runner);

		Test& operator=(std::move_only_function<void()> generator);

	protected:
		void Run() override;

	private:
		std::move_only_function<void()> runner_;
	};

	template<typename T>
	Test<T>::Test(std::string_view name, std::move_only_function<void(const T&)> runner) :
		BaseTest(name),
		runner_(std::move(runner))
	{
	}

	template<typename T>
	Test<T>& Test<T>::operator=(std::move_only_function<void(const T&)> runner)
	{
		runner_ = std::move(runner);
		return *this;
	}

	template<typename T>
	void Test<T>::Run()
	{
	}

	// Template Deductions

	Test(std::string_view, std::move_only_function<void()>) -> Test<void>;

	template<typename T>
	Test(std::string_view, std::move_only_function<void(const T&)>) -> Test<T>;

	// Operators

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
