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
		"instance",
		[](Fixture& fixture) -> Generator
		{
			// We generate a string from the path to the temporary file for comparison.
			const std::filesystem::path temporaryPath = fixture.TempFilePath();
			const std::u8string u8Temp = temporaryPath.u8string();

			const std::string temp(u8Temp.cbegin(), u8Temp.cend());

			co_yield "list parsing"_test = [&](const Requirements& requirements) -> void
			{
				std::array<std::string_view, 4> arguments{"list", "--json", "--file", temp};

				synodic::honesty::log::RingBuffer<std::mutex> sink;

				Instance::Configuration configuration(&sink);
				Instance instance(configuration, arguments);

				const Instance::ListContext* context = instance.GetListContext();

				requirements.Assert(context);
				requirements.Expect(context->outputType == ListOutputType::JSON);
				requirements.Assert(context->file.has_value());
				requirements.Expect(context->file.value() == temporaryPath);

				instance.Execute();

				requirements.Expect(exists(temporaryPath));
			};
		});
	SuiteRegistrar _(SUITE);
}
