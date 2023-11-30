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
	co_yield Set("outer", outerSetGenerator);

	// TODO: Create Tage testing

	// HArdcoded tags

	// co_yield tag("test") / "inner"_set = innerSetGenerator;
	// co_yield skip / "test"_tag / "inner"_set = innerSetGenerator;

	// TODO: Create Log Testing

	// log << "message"

	// TODO: Exception testing
	/*
	 *"exceptions/aborts"_test = [] {
  expect(throws<std::runtime_error>([] { throw std::runtime_error{""}; }))
	<< "throws runtime_error";
  expect(throws([] { throw 0; })) << "throws any exception";
  expect(nothrow([]{})) << "doesn't throw";
  expect(aborts([] { assert(false); }));
};
	 */
};

// Global creation
Suite suite("outer", suiteGenerator);
