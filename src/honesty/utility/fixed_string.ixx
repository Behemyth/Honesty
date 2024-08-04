export module fixed_string;

import std;

template<typename T, typename... Ts>
concept one_of = (false || ... || std::same_as<T, Ts>);

/**
 * @brief A compile-time fixed string
 *
 * @tparam CharT Character type to be used by the string
 * @tparam N The size of the string
 */
export template<typename CharT, std::size_t N, typename Traits = std::char_traits<CharT>>
class basic_fixed_string
{
public:
	CharT data_[N + 1] = {};  // exposition only

	// types
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

	// construction and assignment
	template<std::convertible_to<CharT>... Chars>
		requires(sizeof...(Chars) == N) && (... && !std::is_pointer_v<Chars>)
	constexpr explicit basic_fixed_string(Chars... chars) noexcept :
		data_ {chars..., CharT {}}
	{
	}

	// NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
	consteval explicit(false) basic_fixed_string(const CharT (&txt)[N + 1]) noexcept
	{
		gsl_Expects(txt[N] == CharT {});
		for (std::size_t i = 0; i < N; ++i)
		{
			data_[i] = txt[i];
		}
	}

	template<std::input_iterator It, std::sentinel_for<It> S>
		requires std::convertible_to<std::iter_value_t<It>, CharT>
	constexpr basic_fixed_string(It begin, S end)
	{
		gsl_Expects(std::distance(begin, end) == N);
		for (auto it = data_; begin != end; ++begin, ++it)
		{
			*it = *begin;
		}
	}

	template<std::ranges::input_range R>
		requires std::convertible_to<std::ranges::range_reference_t<R>, CharT>
	constexpr basic_fixed_string(std::from_range_t, R&& r)
	{
		gsl_Expects(std::ranges::size(r) == N);
		for (auto it = data_; auto&& v: std::forward<R>(r))
		{
			*it++ = std::forward<decltype(v)>(v);
		}
	}

	constexpr basic_fixed_string(const basic_fixed_string&) noexcept			= default;
	constexpr basic_fixed_string& operator=(const basic_fixed_string&) noexcept = default;

	// iterator support
	[[nodiscard]] constexpr const_iterator begin() const noexcept
	{
		return data();
	}

	[[nodiscard]] constexpr const_iterator end() const noexcept
	{
		return data() + size();
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

	// capacity
	static constexpr std::integral_constant<size_type, N> size {};
	static constexpr std::integral_constant<size_type, N> length {};
	static constexpr std::integral_constant<size_type, N> max_size {};
	static constexpr std::bool_constant<N == 0> empty {};

	// element access
	[[nodiscard]] constexpr const_reference operator[](size_type pos) const
	{
		gsl_Expects(pos < N);
		return data()[pos];
	}

	[[nodiscard]] constexpr const_reference at(size_type pos) const
	{
		if (pos >= size())
		{
			throw std::out_of_range("basic_fixed_string::at");
		}
		return (*this)[pos];
	}

	[[nodiscard]] constexpr const_reference front() const
	{
		gsl_Expects(!empty());
		return (*this)[0];
	}

	[[nodiscard]] constexpr const_reference back() const
	{
		gsl_Expects(!empty());
		return (*this)[N - 1];
	}

	// modifiers
	constexpr void swap(basic_fixed_string& s) noexcept
	{
		std::swap_ranges(begin(), end(), s.begin());
	}

	// string operations
	[[nodiscard]] constexpr const_pointer c_str() const noexcept
	{
		return data();
	}

	[[nodiscard]] constexpr const_pointer data() const noexcept
	{
		return static_cast<const_pointer>(data_);
	}

	[[nodiscard]] constexpr std::basic_string_view<CharT, Traits> view() const noexcept
	{
		return std::basic_string_view<CharT, Traits>(cbegin(), cend());
	}

	// NOLINTNEXTLINE(*-explicit-conversions, google-explicit-constructor)
	[[nodiscard]] constexpr explicit(false) operator std::basic_string_view<CharT, Traits>() const noexcept
	{
		return view();
	}

	template<std::size_t N2>
	[[nodiscard]] constexpr friend basic_fixed_string<CharT, N + N2, Traits>
		operator+(const basic_fixed_string& lhs, const basic_fixed_string<CharT, N2, Traits>& rhs) noexcept
	{
		CharT txt[N + N2];
		CharT* it = txt;
		for (CharT c: lhs)
		{
			*it++ = c;
		}
		for (CharT c: rhs)
		{
			*it++ = c;
		}
		return basic_fixed_string<CharT, N + N2, Traits>(txt, it);
	}

	[[nodiscard]] constexpr friend basic_fixed_string<CharT, N + 1, Traits>
		operator+(const basic_fixed_string& lhs, CharT rhs) noexcept
	{
		CharT txt[N + 1];
		CharT* it = txt;
		for (CharT c: lhs)
		{
			*it++ = c;
		}
		*it++ = rhs;
		return basic_fixed_string<CharT, N + 1, Traits>(txt, it);
	}

	[[nodiscard]] constexpr friend basic_fixed_string<CharT, 1 + N, Traits>
		operator+(const CharT lhs, const basic_fixed_string& rhs) noexcept
	{
		CharT txt[1 + N];
		CharT* it = txt;
		*it++	  = lhs;
		for (CharT c: rhs)
		{
			*it++ = c;
		}
		return basic_fixed_string<CharT, 1 + N, Traits>(txt, it);
	}

	template<std::size_t N2>
	[[nodiscard]] consteval friend basic_fixed_string<CharT, N + N2 - 1, Traits>
		operator+(const basic_fixed_string& lhs, const CharT (&rhs)[N2]) noexcept
	{
		gsl_Expects(rhs[N2 - 1] == CharT {});
		CharT txt[N + N2];
		CharT* it = txt;
		for (CharT c: lhs)
		{
			*it++ = c;
		}
		for (CharT c: rhs)
		{
			*it++ = c;
		}
		return txt;
	}

	template<std::size_t N1>
	[[nodiscard]] consteval friend basic_fixed_string<CharT, N1 + N - 1, Traits>
		operator+(const CharT (&lhs)[N1], const basic_fixed_string& rhs) noexcept
	{
		gsl_Expects(lhs[N1 - 1] == CharT {});
		CharT txt[N1 + N];
		CharT* it = txt;
		for (size_t i = 0; i != N1 - 1; ++i)
		{
			*it++ = lhs[i];
		}
		for (CharT c: rhs)
		{
			*it++ = c;
		}
		*it++ = CharT();
		return txt;
	}

	// non-member comparison functions
	template<size_t N2>
	[[nodiscard]] friend constexpr bool
		operator==(const basic_fixed_string& lhs, const basic_fixed_string<CharT, N2, Traits>& rhs)
	{
		return lhs.view() == rhs.view();
	}

	template<size_t N2>
	[[nodiscard]] friend consteval bool operator==(const basic_fixed_string& lhs, const CharT (&rhs)[N2])
	{
		gsl_Expects(rhs[N2 - 1] == CharT {});
		return lhs.view() == std::basic_string_view<CharT, Traits>(rhs, rhs + N2 - 1);
	}

	template<size_t N2>
	[[nodiscard]] friend constexpr auto
		operator<=>(const basic_fixed_string& lhs, const basic_fixed_string<CharT, N2, Traits>& rhs)
	{
		return lhs.view() <=> rhs.view();
	}

	template<size_t N2>
	[[nodiscard]] friend consteval auto operator<=>(const basic_fixed_string& lhs, const CharT (&rhs)[N2])
	{
		gsl_Expects(rhs[N2 - 1] == CharT {});
		return lhs.view() <=> std::basic_string_view<CharT, Traits>(rhs, rhs + N2 - 1);
	}

	// inserters and extractors
	friend std::basic_ostream<CharT, Traits>&
		operator<<(std::basic_ostream<CharT, Traits>& os, const basic_fixed_string& str)
	{
		return os << str.c_str();
	}
};

// deduction guides
export template<one_of<char, char8_t, char16_t, char32_t, wchar_t> CharT, std::convertible_to<CharT>... Rest>
basic_fixed_string(CharT, Rest...) -> basic_fixed_string<CharT, 1 + sizeof...(Rest)>;

export template<typename CharT, std::size_t N>
basic_fixed_string(const CharT (&str)[N]) -> basic_fixed_string<CharT, N - 1>;

export template<one_of<char, char8_t, char16_t, char32_t, wchar_t> CharT, std::size_t N>
basic_fixed_string(std::from_range_t, std::array<CharT, N>) -> basic_fixed_string<CharT, N>;

// specialized algorithms
export template<class charT, size_t N, class traits>
constexpr void swap(basic_fixed_string<charT, N, traits>& x, basic_fixed_string<charT, N, traits>& y) noexcept
{
	x.swap(y);
}

// typedef-names
export template<std::size_t N>
using fixed_string = basic_fixed_string<char, N>;
export template<std::size_t N>
using fixed_u8string = basic_fixed_string<char8_t, N>;
export template<std::size_t N>
using fixed_u16string = basic_fixed_string<char16_t, N>;
export template<std::size_t N>
using fixed_u32string = basic_fixed_string<char32_t, N>;
export template<std::size_t N>
using fixed_wstring = basic_fixed_string<wchar_t, N>;

// hash support
export template<std::size_t N>
struct std::hash<fixed_string<N>> : std::hash<std::string_view>
{
};

export template<std::size_t N>
struct std::hash<fixed_u8string<N>> : std::hash<std::u8string_view>
{
};

export template<std::size_t N>
struct std::hash<fixed_u16string<N>> : std::hash<std::u16string_view>
{
};

export template<std::size_t N>
struct std::hash<fixed_u32string<N>> : std::hash<std::u32string_view>
{
};

export template<std::size_t N>
struct std::hash<fixed_wstring<N>> : std::hash<std::wstring_view>
{
};

// formatting support
export template<typename CharT, std::size_t N>
struct std::formatter<basic_fixed_string<CharT, N>> : formatter<std::basic_string_view<CharT>>
{
	template<typename FormatContext>
	auto format(const basic_fixed_string<CharT, N>& str, FormatContext& ctx) const -> decltype(ctx.out())
	{
		return formatter<std::basic_string_view<CharT>>::format(std::basic_string_view<CharT>(str), ctx);
	}
};
