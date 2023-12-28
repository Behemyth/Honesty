
export module synodic.honesty.test:test;

import std;
import function_ref;
import synodic.honesty.test.backend;

export namespace synodic::honesty
{
	class VoidTest;

	using TestGenerator = TestGenerator;
	using TestContext	= TestContext;

	template<typename FuncType, typename RetType>
	concept SameReturn = requires(FuncType func) {
		{
			func()
		} -> std::same_as<RetType>;
	};

	class VoidTest final : public TestBase
	{
	public:
		VoidTest(std::string_view name, std::function_ref<void()> test);

		VoidTest& operator=(std::function_ref<void()> test);

		void Run() const override;

	private:
		std::function_ref<void()> runner_;
	};

	// TODO: Distinguish type overloads
	template<typename Fn>
		requires std::invocable<Fn> && SameReturn<Fn, TestGenerator>
	TestGenerator Test(std::string_view name, Fn&& generator)
	{
		for (const TestBase& test: generator())
		{
			// TODO: inject changes
			co_yield Test(
				"TODO",
				[]()
				{
				});
		}
	}

	template<typename Fn>
		requires std::invocable<Fn> && SameReturn<Fn, void>
	VoidTest Test(std::string_view name, Fn&& generator)
	{
		return VoidTest(name, std::move(generator));
	}

	/**
	 * @brief Strongly typed definition around string_view with construction
	 */
	class [[nodiscard]] TestStub
	{
	public:
		consteval TestStub(std::string_view name) :
			name_(name)
		{
		}

		TestStub(const TestStub& other)				   = delete;
		TestStub(TestStub&& other) noexcept			   = delete;
		TestStub& operator=(const TestStub& other)	   = delete;
		TestStub& operator=(TestStub&& other) noexcept = delete;

		template<typename Fn>
		auto operator=(Fn&& generator) const;

	protected:
		std::string_view name_;
	};

	template<typename Fn>
	auto TestStub::operator=(Fn&& generator) const
	{
		return Test(name_, std::forward<Fn>(generator));
	}

	// Operators

	template<std::invocable<int> Fn>
	[[nodiscard]] TestGenerator operator|(const Fn&& test, const std::ranges::range auto& range)
	{
		for (const auto& value: range)
		{
			co_yield Test("", test);
		}
	}

	template<typename Fn, typename... Types>
		requires(std::invocable<Fn, Types> && ...)
	[[nodiscard]] TestGenerator operator|(const Fn&& test, std::tuple<Types...>&& tuple)
	{
		co_yield Test("", test);
	}

	/**
	 * \brief Allows the static registration of tests in the global scope
	 */
	class suite final
	{
	public:
		suite(std::string_view name, TestGenerator (*generator)());

		suite(const suite& other)	  = delete;
		suite(suite&& other) noexcept = default;

		suite& operator=(const suite& other)	 = delete;
		suite& operator=(suite&& other) noexcept = default;
	};

	suite::suite(std::string_view name, TestGenerator (*generator)())
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
			return TestStub(std::string_view(name, size));
		}

		[[nodiscard]] consteval auto operator""_tag(const char* const name, const std::size_t size)
		{
			return tag(std::string_view(name, size));
		}
	}
}
