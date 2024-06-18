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
		[](const Fixture& fixture) -> Generator
		{
			co_yield "construction"_test = [&](const Requirements& requirements)
			{
				{
					// Empty
					const synodic::honesty::utility::JSON json;

					requirements.Expect(json.Null());
				}
				{
					// Null
					const synodic::honesty::utility::JSON json(nullptr);

					requirements.Expect(json.Null());
				}
				{
					// From value
					const synodic::honesty::utility::JSON json(42);
				}
				{
					// From string
					const synodic::honesty::utility::JSON json("value");
				}
			};

			co_yield "assignment"_test = [&](const Requirements& requirements)
			{
				{
					// From value
					synodic::honesty::utility::JSON json;
					json = nullptr;

					requirements.Expect(json.Null());
				}
				{
					// From value
					synodic::honesty::utility::JSON json;
					json = 42;
				}
				{
					// From string
					synodic::honesty::utility::JSON json;
					json = "value";
				}
			};

			using Data = std::tuple<synodic::honesty::utility::JSON, std::string>;
			std::vector<Data> expectations;
			{
				expectations.push_back({synodic::honesty::utility::JSON(), "{}"});
			}

			co_yield "write"_test = [&](const Requirements& requirements, const Data& data)
			{
				const std::filesystem::path path =
					fixture.SuiteDirectory() / std::format("{}.json", requirements.TestName());

				const std::string output = WriteReadJSON(std::get<0>(data), path);
				const std::string expected = std::get<1>(data);

				requirements.ExpectEquals(output, expected);
			} | expectations;
		});
	SuiteRegistrar _(SUITE);
}
