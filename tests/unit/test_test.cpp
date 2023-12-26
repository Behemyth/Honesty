#include <cstdio>

import std;
import synodic.honesty.test;
import generator;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

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

namespace
{
	using Function = std::move_only_function<TestGenerator()>;

	struct Runner
	{
		explicit Runner(
			Function function,
			const int expected,
			const std::source_location& location = std::source_location::current())
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
	 * \brief Verifies that an empty generator is created
	 */
	auto emptyGenerator = []() -> TestGenerator
	{
		co_return;
	};

	/**
	 * \brief Verifies that a generator with no capture is created
	 */
	auto basicGenerator = []() -> TestGenerator
	{
		co_yield Test(
			"basicGenerator",
			[]
			{
			});
	};

	/**
	 * \brief Verifies that the literal shorthand generates an empty test
	 */
	auto emptyLiteral = []() -> TestGenerator
	{
		co_yield "emptyLiteral"_test = []
		{
		};
	};

	/**
	 * \brief Verifies that the literal shorthand generates an empty test
	 */
	auto emptyRecursive = []() -> TestGenerator
	{
		co_yield std::ranges::elements_of(Test("emptyRecursive", emptyGenerator));
	};

	auto variableCapture = []() -> TestGenerator
	{
		int counter = 0;

		co_yield "variableCapture"_test = [&counter]()
		{
			++counter;
		};
	};
	Runner b(emptyGenerator, 0);
	Runner a(basicGenerator, 1);
	Runner c(emptyLiteral, 1);
	Runner d(emptyRecursive, 0);
	Runner e(variableCapture, 1);
}

// TODO: Create tests

// Tests that creation via literal works
// auto innerSetGenerator = []() -> TestGenerator
//{
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
