import std;

import synodic.honesty.test;
import synodic.honesty.test.mock;
import synodic.honesty.test.api;

import synodic.honesty.log;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"api",
		[](const Fixture& fixture) -> Generator
		{
			const synodic::honesty::log::Logger& root = synodic::honesty::log::RootLogger();
			synodic::honesty::log::Logger logger	  = root.CreateLogger("test");

			MockRunner runner(logger);

			std::vector<std::unique_ptr<Reporter>> reporters;
			reporters.push_back(std::make_unique<MockReporter>(logger));

			co_yield "execute"_test = [&](const Requirements& requirements) -> Generator
			{
				const api::ExecuteParameters baseParameters("execute_test", "", runner, reporters, false, logger);

				co_yield "dry_run"_test = [&](const Requirements& requirements)
				{
					api::ExecuteParameters parameters = baseParameters;
					parameters.dryRun = true;

					const auto result = Execute(parameters);
				};
			};

			/**
			 *	@brief For the 'list' test we need to add a few things to the context. The runner that runs all the
			 *		honesty tests will be reused, but the reporters has two additional considerations:
			 *		1. We need to use the explicit ListReporter to capture the output
			 *		2. We don't want to have the results duplicated with our normal reporters, the one running this test
			 *		As a result, the context is unique to the test, and cannot be managed by the Interface class.
			 */
			co_yield "list"_test = [&](const Requirements& requirements)
			{
				const api::ListParameters parameters("list_test", runner, logger);

				const auto result = List(parameters);

				requirements.ExpectGreater(result.suites.size(), static_cast<std::size_t>(0));
			};
		});
	SuiteRegistrar _(SUITE);
}
