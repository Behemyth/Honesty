import std;
import synodic.honesty.test;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

auto innerSetGenerator = []() -> Generator
{
	co_return;
};

auto suiteGenerator = []() -> Generator
{
	// Tests that creation via literal works
	co_yield "inner"_set = []() -> Generator
	{
		// int count = 0;
		// co_yield Test(
		//	"test",
		//	[&count]
		//	{
		//		++count;
		//	});

		// co_yield "yes"_test = [&count]()
		//{
		//	++count;
		// };

		// co_yield "array"_test = [&count](const auto& parameter)
		//{
		//	++count;
		// } | std::tuple {3u, 4.0f};

		// co_yield "array"_test = [&count]<typename T>(const T& parameter)
		//{
		//	++count;
		// } | std::array {3, 4};

		co_return;
	};

	// Tests that nested set via static creation work
	constexpr Set set("inner", innerSetGenerator);
	co_yield set;
};

// Global creation
Suite suite("outer", suiteGenerator);
