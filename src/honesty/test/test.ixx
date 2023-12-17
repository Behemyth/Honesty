
export module synodic.honesty.test:test;

import std;
import :backend;

export namespace synodic::honesty
{
	// template<std::invocable Fn>
	// auto Test(std::string_view name, Fn&& generator)
	//{
	//	return std::ranges::elements_of(generator());
	// }

	class Test;

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

	class Test final : public TestBase
	{
	public:
		Test(std::string_view name, std::move_only_function<void()> test);

		Test& operator=(std::move_only_function<void()> test);

		void Run() override;

	private:
		std::move_only_function<void()> runner_;
	};

	// Operators
	using Generator = std::generator<TestBase>;

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

	/**
	 * \brief Allows the static registration of tests in the global scope
	 */
	class [[nodiscard]] suite final
	{
	public:
		suite(std::string_view name, std::move_only_function<Generator()> generator);

		suite(const suite& other)	  = delete;
		suite(suite&& other) noexcept = default;

		suite& operator=(const suite& other)	 = delete;
		suite& operator=(suite&& other) noexcept = default;
	};

	class tag
	{
	public:
		explicit consteval tag(std::string_view)
		{
		}

	private:
	};

	tag skip("skip");

	namespace literals
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
}
