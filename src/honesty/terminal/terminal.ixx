// Wrapper around std::format for styled terminal output

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
		explicit text_style(terminal::color8_t color);
		explicit text_style(terminal::color24_t color);

	private:
		std::optional<color_type> fg_color;
		std::optional<color_type> bg_color;
		emphasis ems;
	};

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/utility/format/vformat
	 */
	export inline std::string vformat(const text_style& style, std::string_view fmt, std::format_args args)
	{
	}

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/utility/format/vformat_to
	 */
	export template<std::output_iterator<const char&> OutputIt>
	OutputIt vformat_to(OutputIt out, const text_style& ts, std::string_view fmt, std::format_args args)
	{
	}


	/**
	 * @brief  Overload (1) - https://en.cppreference.com/w/cpp/utility/format/format
	 */
	export template<typename... Args>
	std::string format(const text_style& style, std::format_string<Args...> fmt, Args&&... args)
	{
		return vformat(fmt.get(), std::make_format_args(args...));
	}

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/utility/format/format_to
	 */
	export template<std::output_iterator<const char&> OutputIt, typename... Args>
	OutputIt format_to(OutputIt out, const text_style& style, std::format_string<Args...> fmt, Args&&... args)
	{
		return vformat_to(out, style, fmt.get(), std::make_format_args(args...));
	}
	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/io/vprint_unicode
	 */
	export inline void vprint_unicode(std::FILE* stream, std::string_view fmt, std::format_args args)
	{
	}

	/**
	 * @brief Overload (2) - https://en.cppreference.com/w/cpp/io/vprint_unicode
	 */
	export void vprint_unicode(std::string_view fmt, std::format_args args)
	{
		vprint_unicode(stdout, fmt, args);
	}

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/io/vprint_nonunicode
	 */
	export void vprint_nonunicode(std::FILE* stream, std::string_view fmt, std::format_args args)
	{
	}

	/**
	 * @brief Overload (2) - https://en.cppreference.com/w/cpp/io/vprint_nonunicode
	 */
	export void vprint_nonunicode(std::string_view fmt, std::format_args args)
	{
		vprint_nonunicode(stdout, fmt, args);
	}

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/io/print
	 */
	export template<typename... Args>
	void print(std::FILE* stream, const text_style& style, std::format_string<Args...> fmt, Args&&... args)
	{
		std::vprint_unicode(stream, fmt.get(), std::make_format_args(args...));

		or

		std::vprint_nonunicode(stream, fmt.get(), std::make_format_args(args...));
	}

	/**
	 * @brief Overload (2) - https://en.cppreference.com/w/cpp/io/print
	 */
	export template<typename... Args>
	void print(const text_style& style, std::format_string<Args...> fmt, Args&&... args)
	{
		return print(stdout, style, fmt, std::forward<Args>(args)...);
	}

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/io/println
	 */
	export template<class... Args>
	void println(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args)
	{
		print(stream, "{}\n", std::format(fmt, args...));
	}

	/**
	 * @brief Overload (2) - https://en.cppreference.com/w/cpp/io/println
	 */
	export template<class... Args>
	void println(std::format_string<Args...> fmt, Args&&... args)
	{
		println(stdout, fmt, args...);
	}

}
