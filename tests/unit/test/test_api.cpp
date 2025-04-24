import std;

import synodic.honesty.test;
import synodic.honesty.test.mock;
import synodic.honesty.test.api;

import synodic.honesty.log;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	Suite SUITE(
		"api",
		[](const Fixture& fixture) -> Generator
		{
			const honesty::log::Logger& root = honesty::log::RootLogger();
			honesty::log::Logger logger = root.CreateLogger("test");

			MockRunner runner(logger);

			std::vector<std::unique_ptr<Reporter>> reporters;
			reporters.push_back(std::make_unique<MockReporter>(logger));

			co_yield "execute"_test = [&]() -> Generator
			{
				const std::string header;
				const api::ExecuteParameters
					baseParameters("execute_test", "", runner, reporters, false, header, logger);

				co_yield "dry_run"_test = [&](const Requirements& requirements)
				{
					api::ExecuteParameters parameters = baseParameters;
					parameters.dryRun = true;

					const auto result = Execute(parameters);

					requirements.Expect(result.success);
				};
			};

			/**
			 *	@brief For the 'list' test we need to add a few things to the context. The runner that runs all the
			 *		honesty tests will be reused, but the reporters have two additional considerations:
			 *		1. We need to use the explicit ListReporter to capture the output
			 *		2. We don't want to have the results duplicated with our normal reporters, the one running this test
			 *		As a result, the context is unique to the test, and cannot be managed by the Interface class.
			 */
			co_yield "list"_test = [&](const Requirements& requirements)
			{
				const std::string header;
				const api::ListParameters parameters("list_test", runner, header, logger);

				const auto result = List(parameters);

				requirements.Expect(not result.suites.empty());

				for (auto& suite: result.suites)
				{
					requirements.Expect(not suite.tests.empty(), std::format("Suite '{}' had no tests", suite.name));
				}
			};
		});
	SuiteRegistrar _(SUITE);
}
