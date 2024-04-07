export module synodic.honesty.test:logger;

import synodic.honesty.terminal;
import std;

namespace synodic::honesty::test
{
	export template<typename T, typename... Args>
	concept logger_type = requires(
		T type,
		const terminal::text_style& style,
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
