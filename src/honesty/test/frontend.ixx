export module synodic.honesty.test:test;

import std;

import synodic.honesty.test.backend;

import function_ref;

namespace synodic::honesty::test
{
	// Built-in tags
	export constexpr Tag RUN("run");
	export constexpr Tag SKIP("skip");
	export constexpr Tag FAIL("fail");
	export constexpr Tag TODO("todo");

	class TestLiteral
	{
	public:
		explicit(false) consteval TestLiteral(const std::string_view name) :
			name_(name)
		{
		}

		consteval TestLiteral(const std::string_view name, const Tag& tag) :
			name_(name),
			tag_(tag)
		{
		}

		TestLiteral(const TestLiteral& other)						   = delete;
		consteval TestLiteral(TestLiteral&& other) noexcept			   = default;
		TestLiteral& operator=(const TestLiteral& other)			   = delete;
		consteval TestLiteral& operator=(TestLiteral&& other) noexcept = default;

		template<typename Fn>
			requires std::invocable<Fn, const Requirements&> &&
					 std::same_as<void, std::invoke_result_t<Fn, const Requirements&>>
		inline Test operator=(const Fn& test) const
		{
			return Test(name_, tag_, std::function_ref<void(const Requirements&)>(test));
		}

		template<typename Fn>
			requires std::invocable<Fn, const Requirements&> &&
					 std::same_as<Generator, std::invoke_result_t<Fn, const Requirements&>>
		inline Test operator=(const Fn& test) const
		{
			return Test(name_, tag_, std::function_ref<Generator(const Requirements&)>(test));
		}

		auto operator=(Generator&& generator) const
		{
			return std::ranges::elements_of(std::forward<Generator>(generator));
		}

		friend consteval TestLiteral operator/(const Tag& tag, const TestLiteral& test)
		{
			return TestLiteral(test.name_, tag);
		}

	private:
		std::string_view name_;

		Tag tag_;
	};

	export template<typename Fn, std::ranges::input_range R>
		requires std::regular_invocable<Fn&, const Requirements&, std::ranges::range_reference_t<R>>
	Generator operator|(Fn&& function, R&& range)
	{
		int index = 0;
		for (auto&& element: range)
		{
			co_yield Test(
				std::format("{}", index++),
				Tag(),
				[&function, &element](const Requirements& requirements)
				{
					function(requirements, element);
				});
		}
	}

	export template<typename Fn, typename... Types>
		requires(std::regular_invocable<Fn&, const Requirements&, Types &&> && ...)
	Generator operator|(Fn&& function, std::tuple<Types...>&& tuple)
	{
		co_yield std::ranges::elements_of(std::apply(
			[&function](auto&&... args) -> Generator
			{
				int index = 0;
				(co_yield Test(
					 std::format("{}", index++),
					 Tag(),
					 [&function, &args](const Requirements& requirements)
					 {
						 function(requirements, args);
					 }),
				 ...);
			},
			std::forward<std::tuple<Types...>>(tuple)));
	}

	export namespace literals
	{
		[[nodiscard]] consteval auto operator""_test(const char* const name, const std::size_t size) -> TestLiteral
		{
			return TestLiteral(std::string_view(name, size));
		}

		[[nodiscard]] consteval auto operator""_tag(const char* const name, const std::size_t size) -> Tag
		{
			std::string_view tag(name, size);
			return Tag(std::string_view(name, size));
		}
	}
}
