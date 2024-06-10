export module synodic.honesty.utility:string;

import std;

namespace synodic::honesty::utility
{

	export template<typename CharT, std::size_t Size, typename Traits = std::char_traits<CharT>>
	class BasicFixedString
	{
	public:
		BasicFixedString(const BasicFixedString& other)				   = default;
		BasicFixedString(BasicFixedString&& other) noexcept			   = default;
		BasicFixedString& operator=(const BasicFixedString& other)	   = default;
		BasicFixedString& operator=(BasicFixedString&& other) noexcept = default;

	private:
		static constexpr std::size_t VIEW_SIZE = Size - 1;

	public:
		using StringViewType = std::basic_string_view<CharT, Traits>;

		explicit(false) constexpr BasicFixedString(const CharT (&str)[Size])
		{
			std::copy(std::begin(str), std::end(str), std::begin(data_));
		}

		explicit constexpr operator std::basic_string_view<CharT>() const noexcept
		{
			return {data_.data(), VIEW_SIZE};
		}

	private:
		std::array<CharT, Size> data_;
	};

	export template<typename CharT, typename Traits, std::size_t SizeLeft, std::size_t SizeRight>
	[[nodiscard]] constexpr bool operator==(
		const BasicFixedString<CharT, SizeLeft, Traits>& lhs,
		const BasicFixedString<CharT, SizeRight, Traits>& rhs)
	{
		if constexpr (SizeLeft != SizeRight)
		{
			return false;
		}

		using ViewType = typename BasicFixedString<CharT, SizeLeft, Traits>::StringViewType;
		return static_cast<ViewType>(rhs) == lhs;
	}

	export template<typename CharT, typename Traits, std::size_t SizeLeft>
	[[nodiscard]] constexpr bool operator==(
		const BasicFixedString<CharT, SizeLeft, Traits>& lhs,
		std::type_identity_t<std::basic_string_view<CharT, Traits>> rhs)
	{
		using ViewType = typename BasicFixedString<CharT, SizeLeft, Traits>::StringViewType;
		return static_cast<ViewType>(lhs) == rhs;
	}

	export template<typename CharT, typename Traits, std::size_t SizeLeft, std::size_t SizeRight>
	[[nodiscard]] constexpr auto operator<=>(
		const BasicFixedString<CharT, SizeLeft, Traits>& lhs,
		const BasicFixedString<CharT, SizeRight, Traits>& rhs)
	{
		using ViewType = typename BasicFixedString<CharT, SizeLeft, Traits>::StringViewType;
		return static_cast<ViewType>(lhs) <=> rhs;
	}

	export template<typename CharT, typename Traits, std::size_t SizeLeft>
	[[nodiscard]] constexpr auto operator<=>(
		const BasicFixedString<CharT, SizeLeft, Traits>& lhs,
		std::type_identity_t<std::basic_string_view<CharT, Traits>> rhs)
	{
		using ViewType = typename BasicFixedString<CharT, SizeLeft, Traits>::StringViewType;
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

export template<typename CharT, std::size_t Size, typename Traits>
struct std::formatter<synodic::honesty::utility::BasicFixedString<CharT, Size, Traits>, CharT> :
	std::formatter<std::basic_string_view<CharT, Traits>, CharT>
{
	auto format(const synodic::honesty::utility::BasicFixedString<CharT, Size, Traits>& string, auto& context) const
	{
		return std::formatter<std::basic_string_view<CharT, Traits>, CharT>::format(std::format("{}", string), context);
	}
};
