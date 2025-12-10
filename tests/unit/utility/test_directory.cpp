import std;
import synodic.honesty.utility;
import synodic.honesty.test;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	Suite SUITE(
		"directory",
		[]() -> Generator
		{
			co_yield "data_directory_returns_path"_test = [](const Requirements& requirements)
			{
				const auto path = honesty::utility::DataDirectory();

				// Path should not be empty
				requirements.Expect(!path.empty());

				// Path should contain "honesty" somewhere
				const auto pathStr = path.string();
				requirements.Expect(pathStr.find("honesty") != std::string::npos);
			};

			co_yield "benchmark_directory_creates_and_returns_path"_test = [](const Requirements& requirements)
			{
				const auto path = honesty::utility::BenchmarkDirectory();

				// Path should exist (BenchmarkDirectory creates it)
				requirements.Expect(std::filesystem::exists(path));

				// Path should be a directory
				requirements.Expect(std::filesystem::is_directory(path));

				// Path should contain "benchmarks"
				const auto pathStr = path.string();
				requirements.Expect(pathStr.find("benchmarks") != std::string::npos);
			};

			co_yield "ensure_directory_creates_nested"_test = [](const Requirements& requirements)
			{
				const auto tempDir = std::filesystem::temp_directory_path() / "honesty_test_ensure_dir" / "nested" / "path";

				// Clean up if exists from previous run
				std::error_code ec;
				std::filesystem::remove_all(tempDir.parent_path().parent_path(), ec);

				// Should not exist yet
				requirements.Expect(!std::filesystem::exists(tempDir));

				// Create it
				honesty::utility::EnsureDirectory(tempDir);

				// Should exist now
				requirements.Expect(std::filesystem::exists(tempDir));
				requirements.Expect(std::filesystem::is_directory(tempDir));

				// Cleanup
				std::filesystem::remove_all(tempDir.parent_path().parent_path(), ec);
			};

			co_yield "benchmark_directory_is_consistent"_test = [](const Requirements& requirements)
			{
				// Multiple calls should return the same path
				const auto path1 = honesty::utility::BenchmarkDirectory();
				const auto path2 = honesty::utility::BenchmarkDirectory();

				requirements.ExpectEquals(path1.string(), path2.string());
			};
		});
	SuiteRegistrar _(SUITE);
}
