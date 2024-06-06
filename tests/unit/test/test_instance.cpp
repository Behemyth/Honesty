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
		[]() -> Generator
		{
			const std::filesystem::path temporaryPath = std::filesystem::temp_directory_path() / "testList.json";
			const std::u8string u8Temp = temporaryPath.u8string();

			const std::string temp(u8Temp.cbegin(), u8Temp.cend());

			co_yield "list parsing"_test = [&](const Requirements& requirements) -> void
			{
				std::array<std::string_view, 4> arguments{"list", "--json", "--file", temp};

				Instance::Configuration configuration;
				Instance instance(configuration, arguments);

				const Instance::ListContext* context = instance.GetListContext();

				requirements.Assert(context);
				requirements.ExpectEquals(context->outputType, ListOutputType::JSON);
				requirements.Assert(context->file.has_value());
				requirements.ExpectEquals(context->file.value(), temporaryPath);

				instance.Execute();
			};
		});
	SuiteRegistrar _(SUITE);
}
