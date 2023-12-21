#include <cassert>
#include <cstdio>

import std;
import synodic.honesty.test;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

template<class... Args>
void expect(bool value, const std::source_location location, std::format_string<Args...> fmt, Args&&... args)
{
	std::println(
		stderr,
		"Test Failed: File({}), Line({})",
		location.file_name(),
		location.line());
	std::print(stderr, "    ");
	std::println(stderr, fmt, std::forward<Args>(args)...);
}

namespace
{
	auto basicGenerator = []() -> TestGenerator
	{
		co_return;
	};

	using Function = std::move_only_function<TestGenerator()>;

	struct Runner
	{
		explicit Runner(
			Function function,
			const int expected,
			const std::source_location& location =
				std::source_location::current())
		{
			int count = 0;
			for (const auto& test: function())
			{
				++count;
			}

			expect(count == expected, location, "{} values were generated. {} was expected", count, expected);
		}
	};

	/**
	 * \brief Verifies that an empty generator can be executed
	 */
	Runner runner(basicGenerator, 0);
}

// Tests that creation via literal works
// auto innerSetGenerator = []() -> TestGenerator
//{
//	int count = 0;
//	co_yield Test(
//		"test",
//		[]
//		{
//		});

//	// co_yield "yes"_test = [&count]()
//	//{
//	//	++count;
//	// };

//	// co_yield "array"_test = [&count](const auto& parameter)
//	//{
//	//	++count;
//	// } | std::tuple{3u, 4.0f};

//	// co_yield "array"_test = [&count]<typename T>(const T& parameter)
//	//{
//	//	++count;
//	// } | std::array{3, 4};
//};

// co_yield Test("top level test", outerSetGenerator);
// co_yield Test("inner level test", innerSetGenerator);
