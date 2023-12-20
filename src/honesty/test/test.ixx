
export module synodic.honesty.test:test;

import std;
import generator;
import synodic.honesty.test.backend;

export namespace synodic::honesty
{

	using honesty::Generator;

	auto Test(std::string_view name, Generator (*generator)())
	{
		return std::ranges::elements_of(generator());
	}

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

		auto operator=(Generator (*generator)()) const;

	protected:
		std::string_view name_;
	};

	class Test final : public TestBase
	{
	public:
		Test(std::string_view name, void (*test)());

		Test& operator=(void (*test)());

		void Run() const override;

	private:
		void (*runner_)();
	};

	// Operators

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
	class suite final
	{
	public:
		suite(std::string_view name, std::generator<TestBase> (*generator)());

		suite(const suite& other)	  = delete;
		suite(suite&& other) noexcept = default;

		suite& operator=(const suite& other)	 = delete;
		suite& operator=(suite&& other) noexcept = default;
	};

	suite::suite(std::string_view name, std::generator<TestBase> (*generator)())
	{
		Registry::Add(suite_data(name, generator));
	}

	class tag
	{
	public:
		explicit consteval tag(std::string_view)
		{
		}

	private:
	};

	tag skip("skip");

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
	constexpr auto
		expect_throw(const T& expression, const std::source_location& location = std::source_location::current())
	{
		return expression;
	}

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
