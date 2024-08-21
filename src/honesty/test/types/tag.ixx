export module synodic.honesty.test.types:tag;

import std;
import fixed_string;

namespace synodic::honesty::test
{
	export template<std::size_t N>
	class Tag
	{
		static_assert(N > 0);

		using const_iterator = typename std::array<std::string_view, N>::const_iterator;

	public:
		explicit consteval Tag(
			std::convertible_to<std::string_view> auto tag,
			std::convertible_to<std::string_view> auto... tags) :
			tags_ {static_cast<std::string_view>(tag), static_cast<std::string_view>(tags)...}
		{
		}

		template<typename... Args>
			requires(std::convertible_to<Args, std::string_view> && ...)
		explicit consteval Tag(std::tuple<Args...> tags) :
			tags_ {std::apply(
				[](auto... tags)
				{
					return std::array<std::string_view, N> {tags...};
				},
				tags)}
		{
		}

		template<std::size_t RSize>
		consteval Tag<N + RSize> operator/(Tag<RSize> tag) const
		{
			return Tag<(N + RSize)>(std::tuple_cat(tags_, tag.tags_));
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
			bool result = std::ranges::contains(a.tags_, b);
			return result;
		}

	private:
		template<std::size_t>
		friend class Tag;

		std::array<std::string_view, N> tags_;
	};

	export Tag(std::string_view) -> Tag<1>;

	export template<typename... T>
	Tag(std::string_view, T...) -> Tag<1 + sizeof...(T)>;
}
