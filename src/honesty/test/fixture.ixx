export module synodic.honesty.test:fixture;

import std;

// TODO: Implement user fixture extensions.

namespace synodic::honesty::test
{

	/**
	 * @brief A set of thread-safe, stateful utilities for testing. Data fixtures can be implemented by the user's tests
	 *	themselves.
	 */
	export class Fixture
	{
	public:
		Fixture()  = default;
		~Fixture() = default;


		Fixture(const Fixture& other)				 = delete;
		Fixture(Fixture&& other) noexcept			 = delete;
		Fixture& operator=(const Fixture& other)	 = delete;
		Fixture& operator=(Fixture&& other) noexcept = delete;

		auto TempDirectoryPath() -> std::filesystem::path
		{
			// TODO: Add application subdirectory

			return std::filesystem::temp_directory_path();
		}

		auto TempDirectory() -> std::string
		{
			auto path				   = TempDirectoryPath();
			const std::u8string u8Temp = path.u8string();

			return std::string(u8Temp.cbegin(), u8Temp.cend());
		}

		auto TempFilePath() -> std::filesystem::path
		{
			// TODO: Add application subdirectory
			// TODO: Find a better way to generate a temporary file name

			return std::filesystem::temp_directory_path() / std::tmpnam(nullptr);
		}

		auto TempFile() -> std::string
		{
			auto path				   = TempFilePath();
			const std::u8string u8Temp = path.u8string();

			return std::string(u8Temp.cbegin(), u8Temp.cend());
		}

	private:
	};

}
