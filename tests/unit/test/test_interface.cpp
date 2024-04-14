import std;
import synodic.honesty.test;
import synodic.honesty.test.mock;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	auto interfaceGenerator = []() -> generator<TestBase>
	{
		co_yield "list"_test = []()
		{
			MockReporter reporter;
			MockRunner runner;

			Interface::Configuration configuration;

			Interface interface(configuration);

			ListParameters parameters;
			auto result = interface.List(parameters);

			expect_equals(0, 0);
		};
	};

	Suite suite("suite", interfaceGenerator);
	bool result = suite.Register();
}
