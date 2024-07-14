import std;

import synodic.honesty.test;
import synodic.honesty.test.mock;

import synodic.honesty.log;
import synodic.honesty.test.commands;
import synodic.honesty.test.api;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"api",
		[](const Fixture& fixture) -> Generator
		{
			// We generate a string from the path to the temporary file for comparison.
			const std::filesystem::path temporaryPath = fixture.SuiteDirectory() / "passThrough.json";

			/**
			 *	@brief For the 'list' test we need to add a few things to the context. The runner that runs all the
			 *		honesty tests will be reused, but the reporters has two additional considerations:
			 *		1. We need to use the explicit ListReporter to capture the output
			 *		2. We don't want to have the results duplicated with our normal reporters, the one running this test
			 *		As a result, the context is unique to the test, and cannot be managed by the Interface class.
			 */
			co_yield "list"_test = [](const Requirements& requirements)
			{
				const synodic::honesty::log::Logger& root = synodic::honesty::log::RootLogger();

				synodic::honesty::log::Logger logger = root.CreateLogger("test");

				MockRunner runner(logger);

				const api::ListParameters parameters("list_test", runner, logger);
				const auto result = List(parameters);

				requirements.ExpectGreater(result.suites.size(), static_cast<std::size_t>(0));
			};

			co_yield "list_json"_test = [&](const Requirements& requirements)
			{
				const auto path = temporaryPath.generic_string();
				std::array<std::string_view, 5> arguments{"fakeEXE", "list", "--json", "--file", path};

				synodic::honesty::log::RingBuffer<std::mutex> sink;

				Instance::Configuration configuration("instance_test", &sink);
				Instance command(configuration, arguments);

				//requirements.Assert(context);
				//requirements.Expect(context->outputType == ListOutputType::JSON);
				//requirements.Assert(context->file.has_value());
				//requirements.Expect(context->file.value() == temporaryPath);

				command.Execute();

				requirements.Expect(exists(temporaryPath));
			};
		});
	SuiteRegistrar _(SUITE);
}
