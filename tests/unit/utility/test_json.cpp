import std;
import synodic.honesty.utility;
import synodic.honesty.test;
import synodic.honesty.test.mock;
import synodic.honesty.log;
;
using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"json",
		[]() -> Generator
		{
			co_yield "write"_test = [](const Requirements& requirements)
			{
				// std::ofstream file("test.json");
				// JSON data = JSON::Write(file);
			};

			co_yield "parse"_test = [](const Requirements& requirements)
			{
				// std::ifstream file("test.json");
				// JSON data = JSON::Parse(file);
			};

			co_yield "empty"_test = [](const Requirements& requirements)
			{
				synodic::honesty::utility::JSON json;
			};

			co_yield "value"_test = [](const Requirements& requirements)
			{
				synodic::honesty::utility::JSON json;

				json["value"] = 42;

				requirements.ExpectEquals(json["value"], 42);
			};
		});
	SuiteRegistrar _(SUITE);
}
