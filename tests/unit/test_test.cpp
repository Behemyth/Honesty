// These tests are not executed, so they exist to test the compilation of tests

import std;
import synodic.honesty.test;
import synodic.honesty.test.backend;
import generator;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

auto outerSetGenerator = []()
{
};

auto suiteGenerator = []() -> TestGenerator
{
	// Tests that creation via literal works
	//auto innerSetGenerator = []() -> TestGenerator
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

	//co_yield Test("top level test", outerSetGenerator);
	//co_yield Test("inner level test", innerSetGenerator);

	co_return;
};

// Registration
suite suite("test suite", suiteGenerator);
