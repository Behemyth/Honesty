// These tests are not executed, so they exist to test the compilation of tests

import std;
import synodic.honesty.test;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

auto outerSetGenerator = []() -> Generator
{
	co_return;
};

auto suiteGenerator = []() -> Generator
{
	// Tests that creation via literal works
	auto innerSetGenerator = []() -> Generator
	{
		int count = 0;
		// co_yield Test(
		//	"test",
		//	[&count]
		//	{
		//		++count;
		//	});

		co_return;

		// co_yield "yes"_test = [&count]()
		//{
		//	++count;
		// };

		// co_yield "array"_test = [&count](const auto& parameter)
		//{
		//	++count;
		// } | std::tuple{3u, 4.0f};

		// co_yield "array"_test = [&count]<typename T>(const T& parameter)
		//{
		//	++count;
		// } | std::array{3, 4};
	};

	// TODO: Remove Set, replace with recursive test
	co_yield Test("top level test", outerSetGenerator);
};

// Global creation
constexpr suite suite("test suite", suiteGenerator);
