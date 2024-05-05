export module synodic.honesty.utility:string;

import std;

namespace synodic::honesty::utility
{

	export template<typename Char, std::size_t Size, typename Traits = std::char_traits<Char>>
	class BasicFixedString
	{
		static constexpr std::size_t SIZE_WITH_NULL = Size + 1;

	public:
		using StringViewType = std::basic_string_view<Char, Traits>;

		explicit(false) consteval BasicFixedString(const Char (&str)[SIZE_WITH_NULL])
		{
			std::copy(std::begin(str), std::end(str), std::begin(data_));
		}

		explicit consteval operator std::basic_string_view<Char>() const noexcept
		{
			return {data_.data(), Size};
		}

		template<std::size_t SizeOther>
		[[nodiscard]] constexpr bool operator==(const BasicFixedString<Char, SizeOther, Traits>& other)
		{
			if constexpr (Size != SizeOther)
			{
				return false;
			}
			using OtherType = std::decay_t<decltype(other)>;
			using OtherViewType  = typename OtherType::string_view_type;
			return static_cast<OtherViewType>(*this) == other;
		}

		[[nodiscard]] constexpr bool operator==(std::basic_string_view<Char, Traits> other)
		{
			using OtherType = std::decay_t<decltype(other)>;
			using OtherViewType  = typename OtherType::string_view_type;
			return static_cast<OtherViewType>(*this) == other;
		}

		template<std::size_t SizeOther>
		[[nodiscard]] constexpr auto operator<=>(const BasicFixedString<Char, SizeOther, Traits>& other)
		{
			using OtherType = std::decay_t<decltype(other)>;
			using OtherViewType  = typename OtherType::string_view_type;
			return static_cast<OtherViewType>(*this) <=> other;
		}

		[[nodiscard]] constexpr auto operator<=>(std::basic_string_view<Char, Traits> other)
		{
			using OtherType = std::decay_t<decltype(other)>;
			using OtherViewType  = typename OtherType::string_view_type;
			return static_cast<OtherViewType>(*this) <=> other;
		}

	private:
		std::array<Char, SIZE_WITH_NULL> data_;
	};

	export template<std::size_t Size>
	using FixedString = BasicFixedString<char, Size>;

	export template<std::size_t Size>
	using FixedStringU8 = BasicFixedString<char8_t, Size>;

	export template<std::size_t Size>
	using FixedStringU16 = BasicFixedString<char16_t, Size>;

	export template<std::size_t Size>
	using FixedStringU32 = BasicFixedString<char32_t, Size>;

	template<typename Char, std::size_t Size>
	BasicFixedString(const Char (&)[Size]) -> BasicFixedString<Char, Size - 1>;
}
