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
		Fixture(const std::string_view applicationName, const std::string_view suiteName) :
			applicationName_(applicationName),
			suiteName_(suiteName)
		{
			// Generate a unique directory for the suite
			{
				std::string suiteDirectory(suiteName_);
				suiteDirectory += "_";

				// A distribution fitting 16 characters
				std::uniform_int_distribution distribution(0, 15);
				std::random_device device;

				constexpr std::size_t length = 32;
				suiteDirectory.reserve(suiteDirectory.size() + length);

				for (int iteration = 0; iteration < length; ++iteration)
				{
					suiteDirectory += "0123456789ABCDEF"[distribution(device)];
				}

				suiteTempDirectory_ =
					std::filesystem::temp_directory_path() / "honesty" / applicationName_ / suiteDirectory;
			}
		}

		~Fixture() = default;

		Fixture(const Fixture& other)				 = delete;
		Fixture(Fixture&& other) noexcept			 = delete;
		Fixture& operator=(const Fixture& other)	 = delete;
		Fixture& operator=(Fixture&& other) noexcept = delete;

		/**
		 * @brief Returns a path to a directory in the system's temporary directory. The directory is unique to the run
		 *	and suite name. When returned, the directory is created.
		 * @return The path to the directory.
		 */
		[[nodiscard]] auto SuiteDirectory() const -> std::filesystem::path
		{
			std::error_code code;
			create_directories(suiteTempDirectory_, code);

			if (code)
			{
				throw std::runtime_error("Failed to create directory: " + suiteTempDirectory_.generic_string());
			}

			return suiteTempDirectory_;
		}

		/**
		 * @brief Returns a randomly generated name suitable for a temporary file.
		 * @return A random filename.
		 */
		auto TempFileName() const -> std::string
		{
			std::string filename(applicationName_);
			filename += "_";

			// A distribution fitting 16 characters
			std::uniform_int_distribution distribution(0, 15);
			std::random_device device;

			constexpr std::size_t length = 32;
			filename.reserve(filename.size() + length);

			for (int iteration = 0; iteration < length; ++iteration)
			{
				filename += "0123456789ABCDEF"[distribution(device)];
			}

			filename += ".tmp";

			return filename;
		}

		std::string_view ApplicationName() const
		{
			return applicationName_;
		}

		std::string_view SuiteName() const
		{
			return suiteName_;
		}

	private:
		std::string_view applicationName_;
		std::string_view suiteName_;
		std::filesystem::path suiteTempDirectory_;
	};

}
