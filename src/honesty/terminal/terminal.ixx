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

	export enum class emphasis : std::uint8_t
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

	export using color_type = std::variant<color8_t, color24_t>;

	export class text_style
	{
	public:
		explicit text_style(terminal::color8_t color);
		explicit text_style(terminal::color24_t color);

	private:
		std::optional<color_type> fg_color;
		std::optional<color_type> bg_color;
		std::optional<emphasis> ems;
	};

	// Implementation details that should not be exported
	namespace detail
	{
		/**
		 * @brief Application of a text style to a string
		 * @param data
		 * @param style
		 * @param fmt
		 * @param args
		 */
		template<typename OutputIt, typename BackendBuffer, typename CharT>
			requires std::output_iterator<OutputIt, const CharT&>
		void vformat_to(
			OutputIt out,
			const text_style& style,
			std::basic_string_view<CharT> fmt,
			std::basic_format_args<std::basic_format_context<BackendBuffer, std::type_identity_t<CharT>>> args)
		{
			std::vformat_to(out, fmt, args);
		}
	}

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/utility/format/vformat
	 */
	export std::string vformat(const text_style& style, std::string_view fmt, std::format_args args);

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/utility/format/vformat_to
	 */
	export template<std::output_iterator<const char&> OutputIt>
	OutputIt vformat_to(OutputIt out, const text_style& style, std::string_view fmt, std::format_args args)
	{
		detail::vformat_to(out, style, fmt, args);
		return out;
	}

	/**
	 * @brief  Overload (1) - https://en.cppreference.com/w/cpp/utility/format/format
	 */
	export template<typename... Args>
	std::string format(const text_style& style, std::format_string<Args...> fmt, Args&&... args)
	{
		return vformat(style, fmt.get(), std::make_format_args(args...));
	}

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/utility/format/format_to
	 */
	export template<std::output_iterator<const char&> OutputIt, typename... Args>
	OutputIt format_to(OutputIt out, const text_style& style, std::format_string<Args...> fmt, Args&&... args)
	{
		return vformat_to(std::move(out), style, fmt.get(), std::make_format_args(args...));
	}

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/io/print
	 */
	export template<typename... Args>
	void print(std::FILE* stream, const text_style& style, std::format_string<Args...> fmt, Args&&... args)
	{
		std::string data;
		detail::vformat_to(std::back_inserter(data), style, fmt, args);
		std::print(stream, "{}", data);
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
	void println(std::FILE* stream, const text_style& style, std::format_string<Args...> fmt, Args&&... args)
	{
		print(stream, style, "{}\n", std::format(fmt, args...));
	}

	/**
	 * @brief Overload (2) - https://en.cppreference.com/w/cpp/io/println
	 */
	export template<class... Args>
	void println(const text_style& style, std::format_string<Args...> fmt, Args&&... args)
	{
		println(stdout, style, fmt, args...);
	}

}
