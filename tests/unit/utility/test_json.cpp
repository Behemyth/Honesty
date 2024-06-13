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
	auto WriteReadJSON(const synodic::honesty::utility::JSON& json, std::filesystem::path path) -> std::string
	{
		std::stringstream buffer;
		{
			std::ofstream file(path);
			file << json;
		}

		{
			std::ifstream input(path);
			buffer << input.rdbuf();
		}

		return buffer.str();
	}

	Suite SUITE(
		"json",
		[](Fixture& fixture) -> Generator
		{
			co_yield "empty"_test = [&](const Requirements& requirements)
			{
				const std::filesystem::path path = fixture.TempFilePath();
				const synodic::honesty::utility::JSON json;

				const std::string value = WriteReadJSON(json, path);
				const std::string expected = "{}";

				requirements.ExpectEquals(value, expected);
			};

			co_yield "value"_test = [&](const Requirements& requirements)
			{
				const std::filesystem::path path = fixture.TempFilePath();
				synodic::honesty::utility::JSON json;

				json["value"] = 42;

				const int storedValue = static_cast<int>(json["value"]);

				requirements.ExpectEquals(storedValue, 42);

				const std::string value = WriteReadJSON(json, path);
				const std::string expected =
					"{\n"
					"	\"value\": 42\n"
					"}";

				requirements.ExpectEquals(value, expected);
			};
		});
	SuiteRegistrar _(SUITE);
}
