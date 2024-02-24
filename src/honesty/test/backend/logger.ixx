export module synodic.honesty.test.backend:logger;

import std;
import synodic.honesty.terminal;

export namespace synodic::honesty
{
	template<typename T, typename... Args>
	concept logger_type = requires(
		T type,
		const synodic::honesty::terminal::text_style& style,
		std::format_string<Args...> fmt,
		Args... args) {
		{
			type.log(fmt, args...)
		} -> std::same_as<void>;

		{
			type.log(style, fmt, args...)
		} -> std::same_as<void>;
	};
}
