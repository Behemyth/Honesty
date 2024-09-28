export module synodic.honesty.utility:inplace_string;

import std;
import :traits;

namespace std
{

	/**
	 * @brief A string with an internal buffer. Mimics the api of std::basic_string while supporting modifications.
	 *	Implements the interface of std::fixed_string (p3094) while allowing for modifications and a smaller storage
	 *	footprint.
	 */
	export template<typename CharT, std::size_t N, typename Traits = std::char_traits<CharT>>
	class basic_inplace_string
	{
	public:
		using traits_type			 = Traits;
		using value_type			 = CharT;
		using pointer				 = value_type*;
		using const_pointer			 = const value_type*;
		using reference				 = value_type&;
		using const_reference		 = const value_type&;
		using const_iterator		 = const value_type*;
		using iterator				 = const_iterator;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using reverse_iterator		 = const_reverse_iterator;
		using size_type				 = std::size_t;
		using difference_type		 = std::ptrdiff_t;

		constexpr basic_inplace_string() noexcept = default;

		template<std::convertible_to<CharT>... Chars>
			requires(sizeof...(Chars) <= N) && (... && !std::is_pointer_v<Chars>)
		constexpr explicit basic_inplace_string(Chars... chars) noexcept :
			data_ {chars..., CharT {}}
		{
		}

		template<std::size_t Size>
			requires(Size <= N)
		consteval explicit(false) basic_inplace_string(const CharT (&data)[Size + 1]) noexcept
		{
			std::copy_n(data, Size, data_.begin());
			std::fill(data_.begin() + Size, data_.end(), CharT());	// Will always fill at least the built-in terminator
		}

		template<std::input_iterator It, std::sentinel_for<It> S>
			requires std::convertible_to<std::iter_value_t<It>, char>
		constexpr basic_inplace_string(It begin, S end)
		{
		}

		template<std::ranges::input_range R>
			requires std::convertible_to<std::ranges::range_reference_t<R>, CharT>
		constexpr basic_inplace_string(std::from_range_t, R&& r)
		{
		}

		constexpr basic_inplace_string(const basic_inplace_string&) noexcept			= default;
		constexpr basic_inplace_string& operator=(const basic_inplace_string&) noexcept = default;

		[[nodiscard]] constexpr const_iterator begin() const noexcept
		{
			return data();
		}

		[[nodiscard]] constexpr const_iterator end() const noexcept
		{
			return data() + data_.size();
		}

		[[nodiscard]] constexpr const_iterator cbegin() const noexcept
		{
			return begin();
		}

		[[nodiscard]] constexpr const_iterator cend() const noexcept
		{
			return end();
		}

		[[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator(end());
		}

		[[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
		{
			return const_reverse_iterator(begin());
		}

		[[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
		{
			return rbegin();
		}

		[[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
		{
			return rend();
		}

		constexpr size_type size()
		{
			return data_.size();
		}

		constexpr size_type length()
		{
			return data_.size();
		}

		static constexpr std::integral_constant<size_type, N> max_size {};
		static constexpr std::bool_constant<N == 0> empty {};

		[[nodiscard]] constexpr const_reference operator[](size_type pos) const
		{
			return data_[pos];
		}

		[[nodiscard]] constexpr const_reference at(size_type pos) const
		{
			data_.at(pos);
		}

		[[nodiscard]] constexpr const_reference front() const
		{
			return data_.front();
		}

		[[nodiscard]] constexpr const_reference back() const
		{
			return data_.back();
		}

		[[nodiscard]] constexpr const_pointer c_str() const noexcept
		{
			return data();
		}

		[[nodiscard]] constexpr const_pointer data() const noexcept
		{
			return static_cast<const_pointer>(data_.data());
		}

		[[nodiscard]] constexpr std::basic_string_view<CharT, Traits> view() const noexcept
		{
			return std::basic_string_view<CharT, Traits>(cbegin(), cend());
		}

		[[nodiscard]] constexpr explicit(false) operator std::basic_string_view<CharT, Traits>() const noexcept
		{
			return view();
		}

		template<size_t N2>
		[[nodiscard]] friend constexpr bool
			operator==(const basic_inplace_string& lhs, const basic_inplace_string<CharT, N2, Traits>& rhs)
		{
			return lhs.view() == rhs.view();
		}

		template<size_t N2>
		[[nodiscard]] friend consteval bool operator==(const basic_inplace_string& lhs, const CharT (&rhs)[N2])
		{
			return lhs.view() == std::basic_string_view<CharT, Traits>(rhs, rhs + N2 - 1);
		}

		template<size_t N2>
		[[nodiscard]] friend constexpr auto
			operator<=>(const basic_inplace_string& lhs, const basic_inplace_string<CharT, N2, Traits>& rhs)
		{
			return lhs.view() <=> rhs.view();
		}

		template<size_t N2>
		[[nodiscard]] friend constexpr auto operator<=>(const basic_inplace_string& lhs, const CharT (&rhs)[N2])
		{
			return lhs.view() <=> std::basic_string_view<CharT, Traits>(rhs, rhs + N2 - 1);
		}

		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os, const basic_inplace_string& str)
		{
			return os << str.c_str();
		}

	private:
		std::array<CharT, N + 1> data_;
		synodic::honesty::utility::MinimalIntegerType<N> size_;
	};

	template<typename T, typename... Ts>
	concept one_of = (false || ... || std::same_as<T, Ts>);

	// deduction guides
	export template<one_of<char, char8_t, char16_t, char32_t, wchar_t> CharT, std::convertible_to<CharT>... Rest>
	basic_inplace_string(CharT, Rest...) -> basic_inplace_string<CharT, 1 + sizeof...(Rest)>;

	// export template<typename CharT, std::size_t N>
	// basic_inplace_string(const CharT (&str)[N]) -> basic_fixedbasic_inplace_string_string<CharT, N - 1>;

	export template<one_of<char, char8_t, char16_t, char32_t, wchar_t> CharT, std::size_t N>
	basic_inplace_string(std::from_range_t, std::array<CharT, N>) -> basic_inplace_string<CharT, N>;

	export template<std::size_t N>
	using inplace_string = basic_inplace_string<char, N>;
	export template<std::size_t N>
	using inplace_u8string = basic_inplace_string<char8_t, N>;
	export template<std::size_t N>
	using inplace_u16string = basic_inplace_string<char16_t, N>;
	export template<std::size_t N>
	using inplace_u32string = basic_inplace_string<char32_t, N>;
	export template<std::size_t N>
	using inplace_wstring = basic_inplace_string<wchar_t, N>;

}

export template<std::size_t N>
struct std::hash<std::inplace_string<N>> : std::hash<std::string_view>
{
};

export template<std::size_t N>
struct std::hash<std::inplace_u8string<N>> : std::hash<std::u8string_view>
{
};

export template<std::size_t N>
struct std::hash<std::inplace_u16string<N>> : std::hash<std::u16string_view>
{
};

export template<std::size_t N>
struct std::hash<std::inplace_u32string<N>> : std::hash<std::u32string_view>
{
};

export template<std::size_t N>
struct std::hash<std::inplace_wstring<N>> : std::hash<std::wstring_view>
{
};

export template<typename CharT, std::size_t N>
struct std::formatter<std::basic_inplace_string<CharT, N>> : formatter<std::basic_string_view<CharT>>
{
	template<typename FormatContext>
	auto format(const std::basic_inplace_string<CharT, N>& str, FormatContext& ctx) const -> decltype(ctx.out())
	{
		return formatter<std::basic_string_view<CharT>>::format(std::basic_string_view<CharT>(str), ctx);
	}
};
