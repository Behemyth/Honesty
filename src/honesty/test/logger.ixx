export module synodic.honesty.test.logger;

export import :stdout;
import std;

export namespace synodic::honesty
{
	template<typename T>
	concept is_logger = true;

	template<class... Args>
	void log(std::format_string<Args...> fmt, Args&&... args)
	{
	}
}
