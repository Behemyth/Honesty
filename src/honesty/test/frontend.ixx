export module synodic.honesty.test:test;

import std;

import synodic.honesty.test.types;

import function_ref;

namespace synodic::honesty::test
{
	class TestLiteral
	{
	public:
		explicit(false) consteval TestLiteral(const std::string_view name) :
			name_(name)
		{
		}

		TestLiteral(const TestLiteral& other)				 = delete;
		TestLiteral(TestLiteral&& other) noexcept			 = delete;
		TestLiteral& operator=(const TestLiteral& other)	 = delete;
		TestLiteral& operator=(TestLiteral&& other) noexcept = delete;

		template<typename Fn>
			requires std::invocable<Fn, const Requirements&> &&
					 std::same_as<void, std::invoke_result_t<Fn, const Requirements&>>
		inline Test operator=(const Fn& test) const
		{
			return Test(name_, std::function_ref<void(const Requirements&)>(test));
		}

		template<typename Fn>
			requires std::invocable<Fn, const Requirements&> &&
					 std::same_as<Generator, std::invoke_result_t<Fn, const Requirements&>>
		inline Test operator=(const Fn& test) const
		{
			return Test(name_, std::function_ref<Generator(const Requirements&)>(test));
		}

		auto operator=(Generator&& generator) const
		{
			return std::ranges::elements_of(std::forward<Generator>(generator));
		}

	private:
		std::string_view name_;
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
					 [&function, &args](const Requirements& requirements)
					 {
						 function(requirements, args);
					 }),
				 ...);
			},
			std::forward<std::tuple<Types...>>(tuple)));
	}

	export template<std::size_t N>
	class Tag
	{
		static_assert(N > 0);

		using const_iterator = typename std::array<std::string_view, N>::const_iterator;

	public:
		explicit consteval Tag(
			std::convertible_to<std::string_view> auto t,
			std::convertible_to<std::string_view> auto... tags) :
			tags_ {t, tags...}
		{
		}

		template<std::size_t RSize>
		consteval Tag<N + RSize> operator/(Tag<RSize> tag) const
		{
			Tag<N + RSize> result(tags_[0]);

			// Concat
			std::size_t index = 0;

			for (auto& element: tag.tags_)
			{
				result.tags_[index] = std::move(element);
				++index;
			}
			for (auto& element: tags_)
			{
				result.tags_[index] = std::move(element);
				++index;
			}

			return result;
		}

		consteval std::size_t Size() const noexcept
		{
			return tags_.size();
		}

		constexpr const_iterator begin() const noexcept
		{
			return tags_.begin();
		}

		constexpr const_iterator end() const noexcept
		{
			return tags_.end();
		}

		template<std::size_t OtherSize>
		friend constexpr bool operator==(const Tag& a, const Tag<OtherSize>& b)
		{
			// TODO: Replace with range algorithm
			for (auto& tag: b)
			{
				if (std::ranges::contains(a.tags_, tag))
				{
					return true;
				}
			}
			return false;
		}

		friend constexpr bool operator==(const Tag& a, std::string_view b)
		{
			return std::ranges::contains(a.tags_, b);
		}

	private:
		template<std::size_t>
		friend class Tag;

		std::array<std::string_view, N> tags_;
	};

	export Tag(std::string_view) -> Tag<1>;

	export template<typename... T>
	Tag(std::string_view, T...) -> Tag<1 + sizeof...(T)>;

	export constexpr Tag RUN("run");
	export constexpr Tag SKIP("skip");
	export constexpr Tag FAIL("fail");

	export namespace literals
	{
		[[nodiscard]] consteval auto operator""_test(const char* const name, const std::size_t size)
		{
			return TestLiteral(std::string_view(name, size));
		}

		[[nodiscard]] consteval auto operator""_tag(const char* const name, const std::size_t size)
		{
			return Tag(std::string_view(name, size));
		}
	}
}
