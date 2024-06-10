// Wrapper around std::format for styled terminal output

module;

#include <cstdio>

export module synodic.honesty.log:colour;

import std;

namespace synodic::honesty::log
{
	export struct Colour8
	{
		explicit constexpr Colour8(const std::uint8_t code) :
			code(code)
		{
		}

		std::uint8_t code;

		friend auto operator<=>(const Colour8&, const Colour8&) = default;
	};

	export struct Colour24
	{
		explicit constexpr Colour24(const std::uint32_t hexCode) :
			red((hexCode >> 16) & 0xFF),
			green((hexCode >> 8) & 0xFF),
			blue(hexCode & 0xFF)
		{
		}

		constexpr Colour24(const std::uint8_t red, const std::uint8_t green, const std::uint8_t blue) :
			red(red),
			green(green),
			blue(blue)
		{
		}

		std::uint8_t red;
		std::uint8_t green;
		std::uint8_t blue;

		friend auto operator<=>(const Colour24&, const Colour24&) = default;
	};

	/**
	 * @brief Supported SGR parameter mask
	 */
	export enum class Attribute : std::uint8_t
	{
		BOLD	  = 1,
		FAINT	  = 1 << 1,
		ITALIC	  = 1 << 2,
		UNDERLINE = 1 << 3,
		BLINK	  = 1 << 4,
		REVERSE	  = 1 << 5,
		CONCEAL	  = 1 << 6,
		STRIKE	  = 1 << 7,
	};

	constexpr size_t ATTRIBUTE_COUNT = std::numeric_limits<std::underlying_type_t<Attribute>>::digits;

	constexpr std::array<std::uint8_t, ATTRIBUTE_COUNT> ATTRIBUTE_ANSI_CODES = {
		1,	// bold
		2,	// faint
		3,	// italic
		4,	// underline
		5,	// blink
		7,	// reverse
		8,	// conceal
		9,	// strike
	};

	/**
	 * @brief ANSI color escape codes
	 */
	enum class TerminalColor : std::uint8_t
	{
		BLACK = 30,
		RED,
		GREEN,
		YELLOW,
		BLUE,
		MAGENTA,
		CYAN,
		WHITE,
		BRIGHT_BLACK = 90,
		BRIGHT_RED,
		BRIGHT_GREEN,
		BRIGHT_YELLOW,
		BRIGHT_BLUE,
		BRIGHT_MAGENTA,
		BRIGHT_CYAN,
		BRIGHT_WHITE
	};

	export using ColorType = std::variant<Colour8, Colour24>;

	export class TextStyle
	{
	public:
		constexpr explicit TextStyle(Colour8 color);
		constexpr explicit TextStyle(Colour24 color);

		constexpr std::optional<ColorType> Foreground() const;
		constexpr std::optional<ColorType> Background() const;
		constexpr std::uint8_t AttributeMask() const;

		friend auto operator<=>(const TextStyle&, const TextStyle&) = default;

	private:
		std::optional<ColorType> foreground_;
		std::optional<ColorType> background_;
		std::uint8_t attributeMask_;
	};

	constexpr TextStyle::TextStyle(Colour8 color) :
		foreground_(color),
		attributeMask_(0)
	{
	}

	constexpr TextStyle::TextStyle(Colour24 color) :
		foreground_(color),
		attributeMask_(0)
	{
	}

	constexpr std::optional<ColorType> TextStyle::Foreground() const
	{
		return foreground_;
	}

	constexpr std::optional<ColorType> TextStyle::Background() const
	{
		return background_;
	}

	constexpr std::uint8_t TextStyle::AttributeMask() const
	{
		return attributeMask_;
	}

	template<class... Ts>
	struct Overloaded : Ts...
	{
		using Ts::operator()...;
	};

	std::uint8_t ConvertIndexToAnsiCode(std::uint8_t maskIndex)
	{
		return ATTRIBUTE_ANSI_CODES[maskIndex];
	}

	/**
	 * @brief Application of a text style to a string
	 * @param out
	 * @param style
	 * @param fmt
	 * @param args
	 */
	template<typename OutputIt, typename BackendBuffer, typename CharT>
		requires std::output_iterator<OutputIt, const CharT&>
	OutputIt vformat_to_impl(
		OutputIt out,
		const TextStyle& style,
		std::basic_string_view<CharT> fmt,
		std::basic_format_args<std::basic_format_context<BackendBuffer, std::type_identity_t<CharT>>> args)
	{
		OutputIt next = out;

		if (const std::uint8_t attributeMask = style.AttributeMask())
		{
			constexpr size_t attributeCount = std::numeric_limits<std::underlying_type_t<Attribute>>::digits;

			for (int index = 0; index < attributeCount; ++index)
			{
				if (const std::uint8_t mask = static_cast<std::uint8_t>(1 << index); attributeMask & mask)
				{
					next = std::format_to(next, "\x1b[{}m", ConvertIndexToAnsiCode(index));
				}
			}
		}

		if (std::optional<ColorType> foreground = style.Foreground())
		{
			ColorType& colorType = foreground.value();

			std::visit(
				Overloaded {
					[&](Colour8 arg)
					{
						next = std::format_to(next, "\x1b[{}m", arg.code);
					},
					[&](Colour24 arg)
					{
						next = std::format_to(next, "\x1b[38;2;{:03};{:03};{:03}m", arg.red, arg.green, arg.blue);
					}},
				colorType);
		}

		if (std::optional<ColorType> background = style.Background())
		{
			ColorType& colorType = background.value();

			std::visit(
				Overloaded {
					[&](Colour8 arg)
					{
						next = std::format_to(next, "\x1b[{}m", arg.code + 10);
					},
					[&](Colour24 arg)
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
	export std::string vformat(const TextStyle& style, std::string_view fmt, std::format_args args)
	{
		std::string data;
		vformat_to_impl(std::back_inserter(data), style, fmt, args);
		return data;
	}

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/utility/format/vformat_to
	 */
	export template<std::output_iterator<const char&> OutputIt>
	OutputIt vformat_to(OutputIt out, const TextStyle& style, std::string_view fmt, std::format_args args)
	{
		return vformat_to_impl(out, style, fmt, args);
	}

	/**
	 * @brief  Overload (1) - https://en.cppreference.com/w/cpp/utility/format/format
	 */
	export template<typename... Args>
	std::string format(const TextStyle& style, std::format_string<Args...> fmt, Args&&... args)
	{
		return vformat(style, fmt.get(), std::make_format_args(args...));
	}

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/utility/format/format_to
	 */
	export template<std::output_iterator<const char&> OutputIt, typename... Args>
	OutputIt format_to(OutputIt out, const TextStyle& style, std::format_string<Args...> fmt, Args&&... args)
	{
		return vformat_to(std::move(out), style, fmt.get(), std::make_format_args(args...));
	}

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/io/print
	 */
	export template<typename... Args>
	void print(std::FILE* stream, const TextStyle& style, std::format_string<Args...> fmt, Args&&... args)
	{
		std::string data;
		vformat_to_impl(std::back_inserter(data), style, fmt, args);
		std::print(stream, "{}", data);
	}

	/**
	 * @brief Overload (2) - https://en.cppreference.com/w/cpp/io/print
	 */
	export template<typename... Args>
	void print(const TextStyle& style, std::format_string<Args...> fmt, Args&&... args)
	{
		return print(stdout, style, fmt, std::forward<Args>(args)...);
	}

	/**
	 * @brief Overload (1) - https://en.cppreference.com/w/cpp/io/println
	 */
	export template<class... Args>
	void println(std::FILE* stream, const TextStyle& style, std::format_string<Args...> fmt, Args&&... args)
	{
		print(stream, style, "{}\n", std::format(fmt, args...));
	}

	/**
	 * @brief Overload (2) - https://en.cppreference.com/w/cpp/io/println
	 */
	export template<class... Args>
	void println(const TextStyle& style, std::format_string<Args...> fmt, Args&&... args)
	{
		println(stdout, style, fmt, args...);
	}

}

export template<>
struct std::formatter<synodic::honesty::log::Colour24> : std::formatter<std::string_view>
{
	template<typename Context>
	auto format(const synodic::honesty::log::Colour24& colour, Context& context) const
	{
		return std::formatter<string_view>::format(
			std::format("({}, {}, {})", colour.red, colour.green, colour.blue),
			context);
	}
};
