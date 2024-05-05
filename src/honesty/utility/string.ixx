export module synodic.honesty.utility:string;

import std;

namespace synodic::honesty::utility
{

	export template<typename Char, std::size_t Size, typename Traits = std::char_traits<Char>>
	class BasicFixedString
	{
		static constexpr std::size_t VIEW_SIZE = Size - 1;

	public:
		using StringViewType = std::basic_string_view<Char, Traits>;

		explicit(false) constexpr BasicFixedString(const Char (&str)[Size])
		{
			std::copy(std::begin(str), std::end(str), std::begin(data_));
		}

		constexpr operator std::basic_string_view<Char>() const noexcept
		{
			return {data_.data(), VIEW_SIZE};
		}

	private:
		std::array<Char, Size> data_;
	};

	export template<typename Char, typename Traits, std::size_t SizeLeft, std::size_t SizeRight>
	[[nodiscard]] constexpr bool operator==(
		const BasicFixedString<Char, SizeLeft, Traits>& lhs,
		const BasicFixedString<Char, SizeRight, Traits>& rhs)
	{
		if constexpr (SizeLeft != SizeRight)
		{
			return false;
		}

		using ViewType = typename BasicFixedString<Char, SizeLeft, Traits>::StringViewType;
		return static_cast<ViewType>(rhs) == lhs;
	}

	export template<typename Char, typename Traits, std::size_t SizeLeft>
	[[nodiscard]] constexpr bool operator==(
		const BasicFixedString<Char, SizeLeft, Traits>& lhs,
		std::type_identity_t<std::basic_string_view<Char, Traits>> rhs)
	{
		using ViewType = typename BasicFixedString<Char, SizeLeft, Traits>::StringViewType;
		return static_cast<ViewType>(lhs) == rhs;
	}

	export template<typename Char, typename Traits, std::size_t SizeLeft, std::size_t SizeRight>
	[[nodiscard]] constexpr auto operator<=>(
		const BasicFixedString<Char, SizeLeft, Traits>& lhs,
		const BasicFixedString<Char, SizeRight, Traits>& rhs)
	{
		using ViewType = typename BasicFixedString<Char, SizeLeft, Traits>::StringViewType;
		return static_cast<ViewType>(lhs) <=> rhs;
	}

	export template<typename Char, typename Traits, std::size_t SizeLeft>
	[[nodiscard]] constexpr auto operator<=>(
		const BasicFixedString<Char, SizeLeft, Traits>& lhs,
		std::type_identity_t<std::basic_string_view<Char, Traits>> rhs)
	{
		using ViewType = typename BasicFixedString<Char, SizeLeft, Traits>::StringViewType;
		return static_cast<ViewType>(lhs) <=> rhs;
	}

	export template<std::size_t Size>
	using FixedString = BasicFixedString<char, Size>;

	export template<std::size_t Size>
	using FixedStringU8 = BasicFixedString<char8_t, Size>;

	export template<std::size_t Size>
	using FixedStringU16 = BasicFixedString<char16_t, Size>;

	export template<std::size_t Size>
	using FixedStringU32 = BasicFixedString<char32_t, Size>;
}
