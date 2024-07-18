import std;

import synodic.honesty.test;
import synodic.honesty.test.mock;
import synodic.honesty.test.commands;

import synodic.honesty.log;
;
using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"command",
		[](const Fixture& fixture) -> Generator
		{
			// We generate a string from the path to the temporary file for comparison.
			const std::filesystem::path temporaryPath = fixture.SuiteDirectory() / "passThrough.json";

			co_yield "list_json"_test = [&](const Requirements& requirements)
			{
				const auto path = temporaryPath.generic_string();
				std::array<std::string_view, 5> arguments {"fakeEXE", "list", "--json", "--file", path};

				synodic::honesty::log::RingBuffer<std::mutex> sink;

				Instance::Configuration configuration("instance_test", &sink);
				Instance command(configuration, arguments);

				requirements.Expect(context->outputType == ListOutputType::JSON);
				requirements.Assert(context->file.has_value());
				requirements.Expect(context->file.value() == temporaryPath);

				command.Execute();

				requirements.Expect(exists(temporaryPath));
			};
		});
	SuiteRegistrar _(SUITE);
}
