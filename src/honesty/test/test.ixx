
export module synodic.honesty.test:test;

import std;
import function_ref;
import :types;
import :context;
import generator;
import :assert;

namespace synodic::honesty::test
{

	export class Test;
	class TestView;

	export using Generator = std::generator<Test>;

	export constexpr void VerifyTestName(const std::string_view name)
	{
		// Empty check
		if (name.empty())
		{
			throw std::runtime_error(std::format(
				"Empty suite name - {}, {}",
				std::source_location::current().file_name(),
				std::source_location::current().line()));
		}

		// Uppercase check
		std::ranges::for_each(
			name,
			[](const char character)
			{
				// Uppercase check
				if (std::isupper(character))
				{
					throw std::runtime_error(std::format(
						"Uppercase suite name - {}, {}",
						std::source_location::current().file_name(),
						std::source_location::current().line()));
				}

				// Space check
				if (std::isspace(character))
				{
					throw std::runtime_error(std::format(
						"Suite name contains spaces - {}, {}",
						std::source_location::current().file_name(),
						std::source_location::current().line()));
				}

				// Check for non-alphanumeric characters, excluding '_'
				if (!std::isalnum(character) && character != '_')
				{
					throw std::runtime_error(std::format(
						"Non-alphanumeric suite name - {}, {}",
						std::source_location::current().file_name(),
						std::source_location::current().line()));
				}
			});
	}

	class Test
	{
	public:
		explicit(false) constexpr Test(
			const std::string_view name,
			const std::function_ref<void(const Requirements&)> test) :
			name_(name),
			test_(test)
		{
			VerifyTestName(name);
		}

		Test(const Test& other)				   = delete;
		Test(Test&& other) noexcept			   = delete;
		Test& operator=(const Test& other)	   = delete;
		Test& operator=(Test&& other) noexcept = delete;

		std::string_view Name() const
		{
			return name_;
		}

	private:
		friend TestView;

		std::string_view name_;
		std::function_ref<void(const Requirements&)> test_;
	};

	class TestView
	{
	public:
		explicit(false) constexpr TestView(const Test& test) :
			name(test.name_),
			test(test.test_)
		{
		}

		TestView(const TestView& other)				   = delete;
		TestView(TestView&& other) noexcept			   = delete;
		TestView& operator=(const TestView& other)	   = delete;
		TestView& operator=(TestView&& other) noexcept = delete;

		std::string_view name;
		std::function_ref<void(const Requirements&)> test;
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

		Test operator=(const std::function_ref<void(const Requirements&)> test) const
		{
			return Test(name_, test);
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
