// Wrapper around std::format for terminal output

module;

#include <cstdio>

export module synodic.honesty.terminal:terminal;

import std;

export namespace synodic::honesty::terminal
{
	struct color8_t
	{
		std::uint8_t code;
	};

	struct color24_t
	{
		std::uint8_t red;
		std::uint8_t green;
		std::uint8_t blue;
	};

	enum class emphasis : std::uint8_t
	{
		bold		  = 1,
		faint		  = 1 << 1,
		italic		  = 1 << 2,
		underline	  = 1 << 3,
		blink		  = 1 << 4,
		reverse		  = 1 << 5,
		conceal		  = 1 << 6,
		strikethrough = 1 << 7,
	};

	using color_type = std::variant<color8_t, color24_t>;

	class text_style
	{
	public:
		text_style(terminal::color24_t);

	private:
		std::optional<color_type> fg_color;
		std::optional<color_type> bg_color;
		emphasis ems;
	};

	/**
	 * @brief  Type erased version of `vformat` that mirrors std::vformat_to with styling
	 */
	template<typename CharT, std::output_iterator<const CharT&> OutputIt>
		requires std::output_iterator<OutputIt, const CharT&>
	OutputIt vformat_to(
		OutputIt out,
		const text_style& style,
		std::basic_string_view<CharT> fmt,
		std::basic_format_args<std::basic_format_args<std::basic_format_context<OutputIt, CharT>>> args)
	{
	}

	/**
	 * @brief  Mirrors `std::vformat` overload(1) with styling
	 */
	inline std::string vformat(const text_style& style, std::string_view fmt, std::format_args args)
	{
		std::string buffer;
		// vformat_to(buffer.begin(), style, fmt, args);
		return buffer;
	}

	/**
	 * @brief  Mirrors `std::vformat` overload(2) with styling
	 */
	inline std::wstring vformat(const text_style& style, std::wstring_view fmt, std::wformat_args args)
	{
		std::wstring buffer;
		// vformat_to(buffer.begin(), style, fmt, args);
		return buffer;
	}

	export inline void vprint(std::FILE* f, const text_style& style, std::string_view fmt, std::format_args args)
	{
		// auto buf = memory_buffer();
		// vformat_to(buf, style, fmt, args);
		// print(f, FMT_STRING("{}"), std::string_view(buf.begin(), buf.size()));
	}

	export template<typename... Args>
	void print(std::FILE* fileStream, const text_style& style, std::format_string<Args...> fmt, Args&&... args)
	{
		vprint(fileStream, style, fmt, std::make_format_args(args...));
	}

	export template<typename... Args>
	void print(const text_style& style, std::format_string<Args...> fmt, Args&&... args)
	{
		return print(stdout, style, fmt, std::forward<Args>(args)...);
	}

	export template<typename... Args>
	inline auto format(const text_style& style, std::format_string<Args...> fmt, Args&&... args) -> std::string
	{
		return vformat(style, fmt, std::make_format_args(args...));
	}

	export template<typename OutputIt, typename CharT>
		requires std::output_iterator<OutputIt, const CharT&>
	OutputIt vformat_to(OutputIt out, const text_style& ts, std::string_view fmt, std::basic_format_args<CharT> args)
	{
		auto&& buf = get_buffer<char>(out);
		vformat_to(buf, ts, fmt, args);
		return get_iterator(buf, out);
	}

	/**
	 * @brief
	 */
	export template<typename OutputIt, typename CharT, typename... Args>
		requires std::output_iterator<OutputIt, const CharT&>
	OutputIt format_to(OutputIt out, const text_style& style, std::basic_format_string<CharT> fmt, Args&&... args)
	{
		return vformat_to(out, style, fmt, std::make_format_args(args...));
	}
}
