import std;
import synodic.honesty.test;
import function_ref;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	/**
	 * \brief Verifies that an empty generator is created
	 */
	auto emptyGenerator = []() -> generator<TestBase>
	{
		co_return;
	};

	/**
	 * \brief Verifies that a generator with no capture is created
	 */
	auto basicGenerator = []() -> generator<TestBase>
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
	auto emptyLiteral = []() -> generator<TestBase>
	{
		co_yield "emptyLiteral"_test = []
		{
		};
	};

	/**
	 * \brief Verifies that the literal shorthand generates an empty test
	 */
	auto emptyRecursive = []() -> generator<TestBase>
	{
		co_yield Test("emptyRecursive", emptyGenerator);
	};

	/**
	 * \brief Verifies that the literal shorthand generates an empty test
	 */
	auto assignedRecursive = []() -> generator<TestBase>
	{
		co_yield "emptyRecursive"_test = emptyGenerator;
	};

	/**
	 * \brief Applies a tuple to a test such that the test is run for each element in the tuple
	 */
	auto tupleParameterization = []() -> generator<TestBase>
	{
		co_yield "array"_test = [](const auto& parameter)
		{
		} | std::tuple{3u, 4.0f};
	};

	/**
	 * \brief Applies an array to a test such that the test is run for each element in the array
	 */
	auto arrayParameterization = []() -> generator<TestBase>
	{
		co_yield "array"_test = []<typename T>(const T& parameter)
		{
		} | std::array{3, 4};
	};

	auto testSuite = []() -> generator<TestBase>
	{
		co_yield "run"_test = []()
		{
			// Runs through each generator and counts the number of test instances recorded
			auto counter = [](std::function_ref<generator<TestBase>()> function) -> int
			{
				int count = 0;
				for (const auto& test: function())
				{
					++count;
				}

				return count;
			};

			//expect_equals(counter(emptyGenerator), 0);
			//expect_equals(counter(basicGenerator), 1);
			//expect_equals(counter(emptyLiteral), 1);
			//expect_equals(counter(emptyRecursive), 0);
			//expect_equals(counter(assignedRecursive), 0);
			//expect_equals(counter(tupleParameterization), 2);
			//expect_equals(counter(arrayParameterization), 2);
		};
	};

	Suite suite("test", testSuite);
}
