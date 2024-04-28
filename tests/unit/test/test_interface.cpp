import std;
import synodic.honesty.test;
import synodic.honesty.test.mock;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	auto interfaceGenerator = []()
	{
		"list"_test = []()
		{
			MockReporter reporter;
			MockRunner runner;

			Interface::Configuration configuration;

			Interface interface(configuration);

			ListParameters parameters(&runner);
			const auto result = interface.List(parameters);

			ExpectGreater(result.tests.size(), 0);
		};
	};

	Suite suite("suite", interfaceGenerator);
	bool result = RegisterSuite(suite);
}
