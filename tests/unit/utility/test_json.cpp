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
		{
			std::ofstream file(path);
			if (!file.is_open())
			{
				throw std::runtime_error("Failed to open file: " + path.generic_string());
			}
			file << json;
		}

		std::stringstream buffer;

		{
			std::ifstream input(path);
			if (!input.is_open())
			{
				throw std::runtime_error("Failed to open file: " + path.generic_string());
			}
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
				const std::filesystem::path path = fixture.SuiteDirectory() / "empty.json";
				const synodic::honesty::utility::JSON json;

				const std::string value = WriteReadJSON(json, path);
				const std::string expected = "{}";

				requirements.ExpectEquals(value, expected);
			};

			co_yield "value"_test = [&](const Requirements& requirements)
			{
				const std::filesystem::path path = fixture.SuiteDirectory() / "value.json";
				synodic::honesty::utility::JSON json;

				json["value"] = 42;

				const int storedValue = static_cast<int>(json["value"]);

				requirements.ExpectEquals(storedValue, 42);

				const std::string value = WriteReadJSON(json, path);
				const std::string expected =
					"{\n"
					"	\"value\": 42\n"
					"}";

				auto cwd = std::filesystem::current_path();

				requirements.ExpectEquals(value, expected);
			};
		});
	SuiteRegistrar _(SUITE);
}
