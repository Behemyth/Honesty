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

			ListParameters parameters;
			auto result = interface.List(parameters);

			auto size = result.tests.size();
			ExpectGreater(size, 0);
		};
	};

	Suite suite("suite", interfaceGenerator);
	bool result = RegisterSuite(suite);
}
