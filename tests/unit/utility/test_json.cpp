import std;
import synodic.honesty.utility;
import synodic.honesty.test;
import synodic.honesty.test.mock;
import synodic.honesty.log;
;
using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	auto WriteReadJSON(const honesty::utility::JSON& json, std::filesystem::path path) -> std::string
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
					const honesty::utility::JSON json;

					requirements.Expect(json.Null());
				}
				{
					// Null
					const honesty::utility::JSON json(nullptr);

					requirements.Expect(json.Null());
				}
				{
					// From value
					const honesty::utility::JSON json(42.0f);
				}
				{
					// From string
					const honesty::utility::JSON json("value");
				}
				{
					// From value array
					std::vector range {1, 2, 3, 4};
					honesty::utility::JSON json(std::from_range, range);
				}

				{
					// From JSON array
					std::vector<honesty::utility::JSON> range {1, 2, 3, 4};
					honesty::utility::JSON json(range);
				}

				{
					// From map
					std::map<std::string, int> range = {
						{"1", 1},
						{"2", 2},
						{"3", 3},
						{"4", 4}
					};
					honesty::utility::JSON json(std::from_range, range);
				}

				{
					// From JSON map
					std::map<std::string, honesty::utility::JSON> range = {
						{"1", 1},
						{"2", 2},
						{"3", 3},
						{"4", 4}
					};
					honesty::utility::JSON json(range);
				}
			};

			co_yield "assignment"_test = [&](const Requirements& requirements)
			{
				{
					// From value
					honesty::utility::JSON json;
					json = nullptr;

					requirements.Expect(json.Null());
				}
				{
					// From value
					honesty::utility::JSON json;
					json = 42;
				}
				{
					// From string
					honesty::utility::JSON json;
					json = "value";
				}
				{
					// From value array
					std::vector range {1, 2, 3, 4};
					honesty::utility::JSON json;
					json.AssignRange(range);
				}

				{
					// From JSON array
					std::vector<honesty::utility::JSON> range {1, 2, 3, 4};
					honesty::utility::JSON json;
					json = range;
				}

				{
					// From map
					std::map<std::string, int> range = {
						{"1", 1},
						{"2", 2},
						{"3", 3},
						{"4", 4}
					};
					honesty::utility::JSON json;
					json.AssignRange(range);
				}

				{
					// From JSON map
					std::map<std::string, honesty::utility::JSON> range = {
						{"1", 1},
						{"2", 2},
						{"3", 3},
						{"4", 4}
					};
					honesty::utility::JSON json;
					json = range;
				}
			};

			using Data = std::tuple<honesty::utility::JSON, std::string>;
			std::vector<Data> expectations;
			{
				honesty::utility::JSON json;
				expectations.push_back({json, "null"});
			}
			{
				honesty::utility::JSON json;

				json = 42;

				expectations.push_back({json, "42"});
			}
			{
				 honesty::utility::JSON json;

				 json.AssignRange(std::array{1, 2, 3, 4});

				 expectations.push_back({json, "[\n\t1,\n\t2,\n\t3,\n\t4\n]"});
			}
			{
				honesty::utility::JSON json;

				json.AssignRange(std::map<std::string, int> {
					{"1", 1},
					{"2", 2},
					{"3", 3},
					{"4", 4}
				});

				expectations.push_back({json, "{\n\t\"1\": 1,\n\t\"2\": 2,\n\t\"3\": 3,\n\t\"4\": 4\n}"});
			}

			co_yield "write"_test = [&](const Requirements& requirements, const Data& data)
			{
				const std::filesystem::path path =
					fixture.SuiteDirectory() / std::format("{}.json", requirements.TestName());

				const std::string output   = WriteReadJSON(std::get<0>(data), path);
				const std::string expected = std::get<1>(data);

				requirements.ExpectEquals(output, expected);
			} | expectations;

			co_yield "parse_null"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("null");
				requirements.Expect(json.Null());
			};

			co_yield "parse_bool_true"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("true");
				requirements.Expect(json.IsBool());
				requirements.ExpectEquals(json.GetBool(), true);
			};

			co_yield "parse_bool_false"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("false");
				requirements.Expect(json.IsBool());
				requirements.ExpectEquals(json.GetBool(), false);
			};

			co_yield "parse_integer"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("42");
				requirements.Expect(json.IsNumber());
				requirements.ExpectEquals(json.GetNumber(), 42.0);
			};

			co_yield "parse_negative_integer"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("-123");
				requirements.Expect(json.IsNumber());
				requirements.ExpectEquals(json.GetNumber(), -123.0);
			};

			co_yield "parse_float"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("3.14159");
				requirements.Expect(json.IsNumber());
				requirements.Expect(std::abs(json.GetNumber() - 3.14159) < 0.00001);
			};

			co_yield "parse_scientific"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("1.5e10");
				requirements.Expect(json.IsNumber());
				requirements.Expect(std::abs(json.GetNumber() - 1.5e10) < 1e5);
			};

			co_yield "parse_string"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("\"hello world\"");
				requirements.Expect(json.IsString());
				requirements.ExpectEquals(json.GetString(), std::string("hello world"));
			};

			co_yield "parse_string_escapes"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("\"line1\\nline2\\ttab\"");
				requirements.Expect(json.IsString());
				requirements.ExpectEquals(json.GetString(), std::string("line1\nline2\ttab"));
			};

			co_yield "parse_empty_array"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("[]");
				requirements.Expect(json.IsArray());
				requirements.ExpectEquals(json.Size(), std::size_t{0});
			};

			co_yield "parse_array"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("[1, 2, 3, 4]");
				requirements.Expect(json.IsArray());
				requirements.ExpectEquals(json.Size(), std::size_t{4});
				requirements.ExpectEquals(json[0].GetNumber(), 1.0);
				requirements.ExpectEquals(json[3].GetNumber(), 4.0);
			};

			co_yield "parse_nested_array"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("[[1, 2], [3, 4]]");
				requirements.Expect(json.IsArray());
				requirements.ExpectEquals(json.Size(), std::size_t{2});
				requirements.ExpectEquals(json[0][1].GetNumber(), 2.0);
				requirements.ExpectEquals(json[1][0].GetNumber(), 3.0);
			};

			co_yield "parse_empty_object"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("{}");
				requirements.Expect(json.IsObject());
				requirements.ExpectEquals(json.Size(), std::size_t{0});
			};

			co_yield "parse_object"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("{\"name\": \"test\", \"value\": 42}");
				requirements.Expect(json.IsObject());
				requirements.Expect(json.Contains("name"));
				requirements.Expect(json.Contains("value"));
				requirements.ExpectEquals(json["name"].GetString(), std::string("test"));
				requirements.ExpectEquals(json["value"].GetNumber(), 42.0);
			};

			co_yield "parse_nested_object"_test = [&](const Requirements& requirements)
			{
				const auto json = honesty::utility::JSON::Parse("{\"outer\": {\"inner\": \"value\"}}");
				requirements.Expect(json.IsObject());
				requirements.Expect(json["outer"].IsObject());
				requirements.ExpectEquals(json["outer"]["inner"].GetString(), std::string("value"));
			};

			co_yield "parse_complex"_test = [&](const Requirements& requirements)
			{
				const std::string input = R"({
					"benchmarks": {
						"test/benchmark": [
							{
								"timestamp": "2025-01-01T12:00:00",
								"meanNs": 123.456,
								"iterations": 1000
							}
						]
					}
				})";

				const auto json = honesty::utility::JSON::Parse(input);
				requirements.Expect(json.IsObject());
				requirements.Expect(json.Contains("benchmarks"));

				const auto& benchmarks = json["benchmarks"];
				requirements.Expect(benchmarks.IsObject());
				requirements.Expect(benchmarks.Contains("test/benchmark"));

				const auto& entries = benchmarks["test/benchmark"];
				requirements.Expect(entries.IsArray());
				requirements.ExpectEquals(entries.Size(), std::size_t{1});

				const auto& entry = entries[0];
				requirements.ExpectEquals(entry["timestamp"].GetString(), std::string("2025-01-01T12:00:00"));
				requirements.Expect(std::abs(entry["meanNs"].GetNumber() - 123.456) < 0.001);
			};

			co_yield "parse_roundtrip"_test = [&](const Requirements& requirements)
			{
				// Create JSON, dump it, parse it back
				honesty::utility::JSON original;
				original["name"] = "benchmark";
				original["results"]["mean"] = 42.5;
				original["results"]["count"] = 100;

				const std::string dumped = original.Dump();
				const auto parsed = honesty::utility::JSON::Parse(dumped);

				requirements.ExpectEquals(parsed["name"].GetString(), std::string("benchmark"));
				requirements.ExpectEquals(parsed["results"]["mean"].GetNumber(), 42.5);
				requirements.ExpectEquals(parsed["results"]["count"].GetNumber(), 100.0);
			};
		});
	SuiteRegistrar _(SUITE);
}
