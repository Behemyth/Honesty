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
			std::filesystem::path temp = std::filesystem::temp_directory_path();

			co_yield "list"_test =
			         [&temp](const Requirements& requirements, std::string input)
			         {
				         auto argumentsRange = std::views::split(input, " ");

				         std::vector<std::string_view> arguments;

				         for (auto argument: argumentsRange)
				         {
					         arguments.push_back(std::string_view(argument));
				         }

				         Instance::Configuration configuration;
				         Instance instance(configuration, arguments);

				         instance.Execute();
			         } | std::array<std::string, 5>{
				         "list",
				         "list --log",
				         "list --json",
				         std::format("list --log --file {}", temp),
				         std::format("list --json --file {}", temp)};
		});
	SuiteRegistrar _(SUITE);
}
