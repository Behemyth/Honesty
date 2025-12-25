export module synodic.honesty.trace:types;

import std;

namespace honesty::trace
{
	export template<typename Enum>
	concept group_enum =
		std::is_scoped_enum_v<Enum>
		&& requires
		{
			{ Enum::COUNT } -> std::convertible_to<Enum>; // TODO: Replace with reflection, C++26
		};

	/**
	 * @brief Compile-time tracing configuration.
	 *
	 * When disabled, all tracing compiles to nothing via `if constexpr`.
	 * This provides true zero-cost tracing when disabled in production builds.
	 *
	 * Configure via build system defines:
	 *   -DHONESTY_TRACING_DISABLED -> All tracing disabled (zero cost)
	 *   (default)                  -> Tracing enabled
	 *
	 * For debug/release automatic configuration:
	 *   -DHONESTY_TRACING_DEBUG_ONLY -> Enabled in debug, disabled in release (NDEBUG)
	 */
#if defined(HONESTY_TRACING_DISABLED)
	export inline constexpr bool TracingEnabled = false;
#elif defined(HONESTY_TRACING_DEBUG_ONLY) && defined(NDEBUG)
	export inline constexpr bool TracingEnabled = false;
#else
	export inline constexpr bool TracingEnabled = true;
#endif

	/**
	 * @brief Supported attribute value types for span attributes
	 */
	export using AttributeValue = std::variant<
		bool,
		std::int64_t,
		std::uint64_t,
		double,
		std::string_view,
		std::string
	>;

	/**
	 * @brief A key-value pair for span attributes
	 */
	export struct Attribute
	{
		std::string_view key;
		AttributeValue value;

		constexpr Attribute(std::string_view k, AttributeValue v) :
			key(k),
			value(std::move(v))
		{
		}
	};

	/**
	 * @brief Unique identifier for a trace (128-bit)
	 */
	export struct TraceID
	{
		std::array<std::uint8_t, 16> bytes{};

		constexpr bool operator==(const TraceID&) const = default;

		constexpr bool IsValid() const
		{
			return std::ranges::any_of(bytes, [](auto b) { return b != 0; });
		}
	};

	/**
	 * @brief Unique identifier for a span (64-bit)
	 */
	export struct SpanID
	{
		std::array<std::uint8_t, 8> bytes{};

		constexpr bool operator==(const SpanID&) const = default;

		constexpr bool IsValid() const
		{
			return std::ranges::any_of(bytes, [](auto b) { return b != 0; });
		}
	};

	/**
	 * @brief Status of a span
	 */
	export enum class SpanStatus : std::uint8_t
	{
		UNSET,
		OK,
		ERROR
	};

	/**
	 * @brief Kind of span (client, server, internal, etc.)
	 */
	export enum class SpanKind : std::uint8_t
	{
		INTERNAL,
		SERVER,
		CLIENT,
		PRODUCER,
		CONSUMER
	};
}
