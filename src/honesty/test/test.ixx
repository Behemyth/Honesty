
export module synodic.honesty.test:test;

import std;
import :generator;

export namespace synodic::honesty
{
	class Test;

	class TestBase
	{
	public:
		virtual ~TestBase() = default;
		virtual void Run() = 0;

	private:
	};

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

	class Test final : TestBase
	{
	public:
		Test(std::string_view name, std::move_only_function<void()> test);

		Test& operator=(std::move_only_function<void()> test);

		void Run() override;

	private:
		std::move_only_function<void()> runner_;
	};
	 
	// Operators

	using Generator = std::generator<Test>;

	template<std::invocable<int> Fn>
	[[nodiscard]] Generator operator|(const Fn&& test, const std::ranges::range auto& range)
	{
		for (const auto& value: range)
		{
			co_yield Test("", test);
		}
	}

	template<typename Fn, typename... Types>
		requires(std::invocable<Fn, Types> && ...)
	[[nodiscard]] Generator operator|(const Fn&& test, std::tuple<Types...>&& tuple)
	{
		co_yield Test("", test);
	}

}
