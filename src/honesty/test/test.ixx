
export module synodic.honesty.test:test;

import std;
import function_ref;
import :types;
import :context;

namespace synodic::honesty::test
{
	export class Test
	{
	public:
		explicit(false) consteval Test(const std::string_view name) :
			name_(name)
		{
		}

		Test(const Test& other)				   = delete;
		Test(Test&& other) noexcept			   = delete;
		Test& operator=(const Test& other)	   = delete;
		Test& operator=(Test&& other) noexcept = delete;

		void operator=(std::function_ref<void()> test) const
		{
			try
			{
				GetContext().Run(test);
			}
			catch (const AssertException& exception)
			{
			}
		}

	private:
		std::string_view name_;
	};

	export template<typename Fn, std::ranges::input_range V>
		requires std::regular_invocable<Fn&, std::ranges::range_reference_t<V>>
	constexpr auto operator|(Fn&& function, V&& range)
	{
		return [&function, &range]()
		{
			for (auto&& element: range)
			{
				Test("Range") = [&function, &element]()
				{
					function(element);
				};
			}
		};
	}

	export template<typename Fn, typename... Types>
		requires(std::regular_invocable<Fn&, Types &&> && ...)
	constexpr auto operator|(Fn&& function, std::tuple<Types...>&& tuple)
	{
		return [&]()
		{
			std::apply(
				[&function](auto&&... args)
				{
					auto application = [&function](auto&& arg)
					{
						Test("Tuple") = [&function, &arg]()
						{
							function(arg);
						};
					};

					(application(args), ...);
				},
				std::forward<std::tuple<Types...>>(tuple));
		};
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
			return Test(std::string_view(name, size));
		}

		[[nodiscard]] consteval auto operator""_tag(const char* const name, const std::size_t size)
		{
			return Tag(std::string_view(name, size));
		}
	}
}
