// Wrapper around std::format for styled terminal output

module;

#include <cstdio>

export module synodic.honesty.log:colour;

import std;

namespace synodic::honesty::terminal
{
	export struct color8_t
	{
		constexpr color8_t(std::uint8_t code) :
			code(code)
		{
		}

		std::uint8_t code;

		friend bool operator<=>(const color8_t&, const color8_t&) = default;
	};

	export struct color24_t
	{
		constexpr color24_t(std::uint32_t hexCode) :
			red((hexCode >> 16) & 0xFF),
			green((hexCode >> 8) & 0xFF),
			blue(hexCode & 0xFF)
		{
		}

		constexpr color24_t(std::uint8_t red, std::uint8_t green, std::uint8_t blue) :
			red(red),
			green(green),
			blue(blue)
		{
		}

		std::uint8_t red;
		std::uint8_t green;
		std::uint8_t blue;

		friend bool operator<=>(const color24_t&, const color24_t&) = default;
	};

	/**
	 * @brief Supported SGR parameter mask
	 */
	export enum class attribute : std::uint8_t
	{
		bold	  = 1,
		faint	  = 1 << 1,
		italic	  = 1 << 2,
		underline = 1 << 3,
		blink	  = 1 << 4,
		reverse	  = 1 << 5,
		conceal	  = 1 << 6,
		strike	  = 1 << 7,
	};

	/**
	 * @brief ANSI color escape codes
	 */
	enum class terminal_color : std::uint8_t
	{
		black = 30,
		red,
		green,
		yellow,
		blue,
		magenta,
		cyan,
		white,
		bright_black = 90,
		bright_red,
		bright_green,
		bright_yellow,
		bright_blue,
		bright_magenta,
		bright_cyan,
		bright_white
	};

	export using color_type = std::variant<color8_t, color24_t>;

	export class text_style
	{
	public:
		constexpr explicit text_style(terminal::color8_t color);
		constexpr explicit text_style(terminal::color24_t color);

		constexpr std::optional<color_type> Foreground() const;
		constexpr std::optional<color_type> Background() const;
		constexpr std::uint8_t AttributeMask() const;

		friend bool operator<=>(const text_style&, const text_style&) = default;

	private:
		std::optional<color_type> foreground_;
		std::optional<color_type> background_;
		std::uint8_t attributeMask_;
	};

	constexpr text_style::text_style(terminal::color8_t color) :
		foreground_(color),
		attributeMask_(0)
	{
	}

	constexpr text_style::text_style(terminal::color24_t color) :
		foreground_(color),
		attributeMask_(0)
	{
	}

	constexpr std::optional<color_type> text_style::Foreground() const
	{
		return foreground_;
	}

	constexpr std::optional<color_type> text_style::Background() const
	{
		return background_;
	}

	constexpr std::uint8_t text_style::AttributeMask() const
	{
		return attributeMask_;
	}

	template<class... Ts>
	struct overloaded : Ts...
	{
		using Ts::operator()...;
	};

	std::uint8_t ConvertIndexToAnsiCode(std::uint8_t maskIndex);

	/**
	 * @brief Application of a text style to a string
	 * @param data
	 * @param style
	 * @param fmt
	 * @param args
	 */
	template<typename OutputIt, typename BackendBuffer, typename CharT>
		requires std::output_iterator<OutputIt, const CharT&>
	OutputIt vformat_to_impl(
		OutputIt out,
		const text_style& style,
		std::basic_string_view<CharT> fmt,
		std::basic_format_args<std::basic_format_context<BackendBuffer, std::type_identity_t<CharT>>> args)
	{
		OutputIt next = out;

		std::uint8_t attributeMask = style.AttributeMask();
		if (attributeMask)
		{
			constexpr size_t ATTRIBUTE_COUNT = std::numeric_limits<std::underlying_type_t<attribute>>::digits;

			for (int index = 0; index < ATTRIBUTE_COUNT; ++index)
			{
				std::uint8_t mask = static_cast<std::uint8_t>(1 << index);
				if (attributeMask & mask)
				{
					next = std::format_to(next, "\x1b[{}m", ConvertIndexToAnsiCode(index));
				}
			}
		}

		std::optional<color_type> foreground = style.Foreground();
		if (foreground)
		{
			color_type& colorType = foreground.value();

			std::visit(
				overloaded {
					[&](color8_t arg)
					{
						next = std::format_to(next, "\x1b[{}m", arg.code);
					},
					[&](color24_t arg)
					{
						next = std::format_to(next, "\x1b[38;2;{:03};{:03};{:03}m", arg.red, arg.green, arg.blue);
					}},
				colorType);
		}

		std::optional<color_type> background = style.Background();
		if (background)
		{
			color_type& colorType = background.value();

			std::visit(
				overloaded {
					[&](color8_t arg)
					{
						next = std::format_to(next, "\x1b[{}m", arg.code + 10);
					},
					[&](color24_t arg)
					{
						next = std::format_to(next, "\x1b[48;2;{:03};{:03};{:03}m", arg.red, arg.green, arg.blue);
					}},
				colorType);
		}

		next = std::vformat_to(next, fmt, args);
		return std::format_to(next, "\x1b[0m");
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
		return vformat_to_impl(out, style, fmt, args);
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
		vformat_to_impl(std::back_inserter(data), style, fmt, args);
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
