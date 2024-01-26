export module synodic.honesty.test.reporter:stdout;

#include <cstdio>

import std;
import synodic.honesty.test.backend;

export namespace synodic::honesty::reporter
{
	class Console : public Reporter
	{
	public:
		template<class... Args>
		void expect(bool value, const std::source_location location, std::format_string<Args...> fmt, Args&&... args)
		{
			if (!value)
			{
				std::println(stderr, "Test Failed: File({}), Line({})", location.file_name(), location.line());
				std::print(stderr, "    ");
				std::println(stderr, fmt, std::forward<Args>(args)...);
			}
		}

		template<class T, class U>
			requires std::equality_comparable_with<T, U>
		void expect_equals(const T& a, const U& b, const std::source_location& location)
		{
			expect(a == b, location, "{} was expected to equal {}. It did not", a, b);
		}
	};
}
