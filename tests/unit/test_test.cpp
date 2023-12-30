#include <cstdio>

import std;
import synodic.honesty.test;
import function_ref;

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
	using Function = std::function_ref<TestGenerator()>;

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
		co_yield Test("emptyRecursive", emptyGenerator);
	};

	/**
	 * \brief Verifies that the literal shorthand generates an empty test
	 */
	auto assignedRecursive = []() -> TestGenerator
	{
		co_yield "emptyRecursive"_test = emptyGenerator;
	};

	auto tupleParameterization = []() -> TestGenerator
	{
		co_yield "array"_test = [](const auto& parameter)
		{
		} | std::tuple {3u, 4.0f};
	};

	auto arrayParameterization = []() -> TestGenerator
	{
		co_yield "array"_test = []<typename T>(const T& parameter)
		{
		} | std::array {3, 4};
	};

	auto tagSuite = []() -> TestGenerator
	{
		auto counter = [](std::function_ref<TestGenerator()> function) -> int
		{
			int count = 0;
			for (const auto& test: function())
			{
				++count;
			}

			return count;
		};

		expect_equals(counter(emptyGenerator), 0);
		expect_equals(counter(basicGenerator), 1);
		expect_equals(counter(emptyLiteral), 1);
		expect_equals(counter(emptyRecursive), 0);
		expect_equals(counter(assignedRecursive), 0);
		expect_equals(counter(tupleParameterization), 2);
		expect_equals(counter(arrayParameterization), 2);

		co_return;
	};

	suite suite("test suite", tagSuite);
}
