
export module synodic.honesty.test:test;

import std;
import function_ref;
import :types;
import :context;
import generator;

namespace synodic::honesty::test
{

	export class Test;
	export using Generator = std::generator<Test>;

	class Test
	{
	public:
		explicit(false) constexpr Test(const std::string_view name, const std::function_ref<void()> test) :
			name_(name),
			test_(
				[]() -> Generator
				{
					co_return;
				})
		{
		}

		explicit(false) constexpr Test(const std::string_view name, const std::function_ref<Generator()> test) :
			name_(name),
			test_(test)
		{
		}

		Test(const Test& other)				   = delete;
		Test(Test&& other) noexcept			   = delete;
		Test& operator=(const Test& other)	   = delete;
		Test& operator=(Test&& other) noexcept = delete;

	private:
		std::string_view name_;
		std::function_ref<Generator()> test_;
	};

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

		consteval Test operator=(const std::function_ref<void()> test) const
		{
			return Test(name_, test);
		}

		Test operator=(const std::function_ref<Generator()> test) const
		{
			return Test(name_, test);
		}

		Test operator=(Generator&& generator) const
		{
			return Test(
				name_,
				[&generator]
				{
					// TODO: Implement
					//co_yield generator;
				});
		}

	private:
		std::string_view name_;
	};

	export template<typename Fn, std::ranges::input_range V>
		requires std::regular_invocable<Fn&, std::ranges::range_reference_t<V>>
	Generator operator|(Fn&& function, V&& range)
	{
		for (auto&& element: range)
		{
			co_yield Test(
				std::format("{}", element),
				[&function, &element]()
				{
					function(element);
				});
		}
	}

	export template<typename Fn, typename... Types>
		requires(std::regular_invocable<Fn&, Types &&> && ...)
	Generator operator|(Fn&& function, std::tuple<Types...>&& tuple)
	{
		co_yield std::apply(
			[&function](auto&&... args) -> Generator
			{
				auto application = [&function](auto&& arg) -> Generator
				{
					co_yield Test(
						std::format("{}", arg),
						[&function, &arg]()
						{
							function(arg);
						});
				};

				std::array<Generator, sizeof...(args)> generators{application(args)...};
				for (auto& generator: generators)
				{
					// TODO: Use elements_of

					//co_yield generator;
				}
			},
			std::forward<std::tuple<Types...>>(tuple));
	}

	export template<std::size_t Size>
	class Tag
	{
	public:
		explicit consteval Tag(
			std::convertible_to<std::string_view> auto t,
			std::convertible_to<std::string_view> auto... tags) :
			tags_ {t, tags...}
		{
		}

		template<std::size_t RSize>
		consteval Tag<Size + RSize> operator/(Tag<RSize> tag) const
		{
			Tag<Size + RSize> result(tags_[0]);

			// Concat
			std::size_t index = 0;

			for (auto& el: tag.tags_)
			{
				result.tags_[index] = std::move(el);
				++index;
			}
			for (auto& el: tags_)
			{
				result.tags_[index] = std::move(el);
				++index;
			}

			return result;
		}

	private:
		template<std::size_t>
		friend class Tag;

		std::array<std::string_view, Size> tags_;
	};

	export Tag(std::string_view) -> Tag<1>;

	export template<typename... T>
	Tag(std::string_view, T...) -> Tag<1 + sizeof...(T)>;

	export constexpr Tag SKIP("skip");

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
