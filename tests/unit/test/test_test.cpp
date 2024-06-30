import std;
import synodic.honesty.test;
import function_ref;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"test",
		[]() -> Generator
		{
			co_yield "run"_test = [](const Requirements& requirements)
			{
				/**
				 * @brief Verifies that an empty callable works
				 */
				auto emptyGenerator = []() -> Generator
				{
					co_return;
				};

				/**
				 * @brief Verifies that the literal shorthand generates an empty test
				 */
				auto emptyLiteral = []() -> Generator
				{
					co_yield "empty_literal"_test = [](const Requirements& _)
					{
					};
				};

				/**
				 * @brief Applies a tuple to a test such that the test is run for each element in the tuple
				 */
				auto tupleParameterization = []() -> Generator
				{
					co_yield "array"_test = [](const Requirements& _, const auto& parameter)
					{
					} | std::tuple{3u, 4.0f};
				};

				/**
				 * @brief Applies an array to a test such that the test is run for each element in the array
				 */
				auto arrayParameterization = []() -> Generator
				{
					co_yield "array"_test = [](const Requirements& _, int parameter)
					{
					} | std::array{3, 4};
				};

				// Runs through each generator and counts the number of test instances recorded
				auto counter = [](const std::function_ref<Generator()> function) -> int
				{
					int count = 0;
					for (const auto& test: function())
					{
						++count;
					}

					return count;
				};

				requirements.ExpectEquals(counter(emptyGenerator), 0);
				requirements.ExpectEquals(counter(emptyLiteral), 1);
				requirements.ExpectEquals(counter(tupleParameterization), 2);
				requirements.ExpectEquals(counter(arrayParameterization), 2);
			};

			co_yield "run"_test = [](const Requirements& requirements)
			{
			};
		});

	SuiteRegistrar _(SUITE);
}
