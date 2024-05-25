import std;
import synodic.honesty.test;
import synodic.honesty.test.mock;
import synodic.honesty.log;
;
using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"suite",
		[]() -> Generator
		{
			co_yield "list"_test = []()
			{
				const synodic::honesty::log::Logger& root = synodic::honesty::log::RootLogger();

				MockReporter reporter(root.CreateLogger("mock_reporter"));
				MockRunner runner(root.CreateLogger("mock_runner"));

				Interface::Configuration configuration;

				Interface interface(configuration);

				ListParameters parameters(&runner, root.CreateLogger("test"));
				const auto result = interface.List(parameters);

				ExpectGreater(result.tests.size(), 0);
			};
		});
	SuiteRegistrar _(SUITE);
}
