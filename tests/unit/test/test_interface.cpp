import std;
import synodic.honesty.test;
import synodic.honesty.test.mock;
import synodic.honesty.log;
;
using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	auto interfaceGenerator = []() -> Generator
	{
		co_yield "list"_test = []()
		{
			MockReporter reporter;
			MockRunner runner;

			Interface::Configuration configuration;

			Interface interface(configuration);

			const synodic::honesty::log::Logger& root = synodic::honesty::log::RootLogger();

			ListParameters parameters(&runner, root.CreateLogger("test"));
			const auto result = interface.List(parameters);

			ExpectGreater(result.tests.size(), 0);
		};
	};

	Suite suite("suite", interfaceGenerator);
	SuiteRegistrar _(suite);
}
