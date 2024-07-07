module synodic.honesty.test.commands:list;

import std;

import :api.list;
import :command.types;

namespace synodic::honesty::test::command
{
	class List final : public Command
	{
	public:
		explicit List(const Configuration& configuration) :
			applicationName_(configuration.applicationName),
			logger_(configuration.logger),
			outputType_(ListOutputType::LOG)
		{
		}

		~List() override = default;

		static constexpr std::string_view NAME = "list";

		auto Parse(std::span<std::string_view> arguments) -> ParseResult override
		{
			arguments = arguments.subspan(1);

			if (std::ranges::contains(arguments, "--json"))
			{
				outputType_ = ListOutputType::JSON;
			}

			if (auto itr = std::ranges::find(arguments, "--file"); itr != arguments.end())
			{
				if (++itr == arguments.end())
				{
					throw std::invalid_argument("You must give a file name when using the '--file' option");
				}

				file_ = absolute(std::filesystem::current_path() / *itr);

				if (not file_->has_filename())
				{
					throw std::invalid_argument("You must give a valid file name when using the '--file' option");
				}
			}

			ParseResult result("", {});

			return result;
		}

		void Process(ProcessConfiguration& configuration) override
		{
			api::ListParameters parameters(applicationName_, configuration.runner.get(), logger_);
			api::ListResult result = api::List(parameters);

			if (file_)
			{
				const std::filesystem::path& path = file_.value();

				std::ofstream file(path);

				std::error_code code;
				create_directories(path.parent_path(), code);

				if (code)
				{
					throw std::runtime_error("Failed to create directory: " + path.parent_path().string());
				}

				if (!file.is_open())
				{
					throw std::runtime_error("Failed to open file: " + path.generic_string());
				}

				// Clear the file
				file.clear();

				switch (outputType_)
				{
					case ListOutputType::LOG:
					{
						for (auto& suiteDescription: result.suites)
						{
							for (auto& testDescription: suiteDescription.tests)
							{
								file << std::format("{}", testDescription.name);
							}
						}
						break;
					}
					case ListOutputType::JSON:
					{
						utility::JSON json;

						utility::JSON& tests = json["tests"];

						size_t testIndex = 0;

						for (auto& suiteDescription: result.suites)
						{
							for (auto& testDescription: suiteDescription.tests)
							{
								tests[testIndex++] = std::format("{}.{}", suiteDescription.name, testDescription.name);
							}
						}

						file << json;

						break;
					}
				}
			}
		}

	private:
		std::string_view applicationName_;

		std::unique_ptr<Runner> runner_;
		std::reference_wrapper<const log::Logger> logger_;

		ListOutputType outputType_;
		std::optional<std::filesystem::path> file_;
	};
}
