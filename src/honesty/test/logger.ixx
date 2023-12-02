export module synodic.honesty.test.logger;

export import :stdout;
import std;

export namespace synodic::honesty
{
	template<typename T, typename... Args>
	concept is_logger = requires(T type, std::format_string<Args...> fmt, Args... args) {
		{
			type.log(fmt, args...)
		} -> std::same_as<void>;
	};

	template<class... Args>
	void log(std::format_string<Args...> fmt, Args&&... args)
	{
	}
}
