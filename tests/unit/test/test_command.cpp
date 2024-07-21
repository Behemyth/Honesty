import std;

import synodic.honesty.test;
import synodic.honesty.test.mock;
import synodic.honesty.test.commands;

import synodic.honesty.log;

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

				std::stringstream stringStream;
				synodic::honesty::log::OStream<std::mutex> stream(stringStream);

				Instance::Configuration configuration("instance_test", &stream);
				Instance command(configuration, arguments);
				std::optional<command::ListData> data = command.CommandData<command::List>();

				requirements.Assert(data.has_value());
				requirements.Expect(data->outputType == command::ListOutputType::JSON);
				requirements.Assert(data->file.has_value());
				requirements.Expect(data->file.value() == temporaryPath);

				command.Execute();

				requirements.Expect(exists(temporaryPath));
			};
		});
	SuiteRegistrar _(SUITE);
}
