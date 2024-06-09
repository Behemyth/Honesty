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
			co_yield "empty"_test = [](const Requirements& requirements)
			{
				const synodic::honesty::utility::JSON json;

				std::ofstream file("test.json");
				file << json;

				std::ifstream t("file.txt");
				std::stringstream buffer;
				buffer << t.rdbuf();
			};

			co_yield "value"_test = [](const Requirements& requirements)
			{
				synodic::honesty::utility::JSON json;

				json["value"] = 42;

				const int storedValue = static_cast<int>(json["value"]);

				requirements.ExpectEquals(storedValue, 42);
			};
		});
	SuiteRegistrar _(SUITE);
}
