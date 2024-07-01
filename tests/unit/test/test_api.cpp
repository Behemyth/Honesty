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
		"api",
		[]() -> Generator
		{
			/**
			 *	@brief For the 'list' test we need to add a few things to the context. The runner that runs all the
			 *		honesty tests will be reused, but the reporter has two additional considerations:
			 *		1. We need to use the explicit ListReporter to capture the output
			 *		2. We don't want to have the results duplicated with our normal reporter, the one running this test
			 *		As a result, the context is unique to the test, and cannot be managed by the Interface class.
			 */
			co_yield "list"_test = [](const Requirements& requirements)
			{
				const synodic::honesty::log::Logger& root = synodic::honesty::log::RootLogger();

				const Interface::Configuration configuration("interfaceTest");

				Interface api(configuration);

				ListParameters parameters(root.CreateLogger("test"));
				const auto result = api.List(parameters);

				requirements.ExpectGreater(result.suites.size(), static_cast<std::size_t>(0));
			};
		});
	SuiteRegistrar _(SUITE);
}
