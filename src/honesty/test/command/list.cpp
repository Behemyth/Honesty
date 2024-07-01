module synodic.honesty.test:command.list;

import std;

import :api;
import :command.types;

namespace synodic::honesty::test::command
{
	class List final : public Command
	{
	public:
		explicit List(const Configuration& configuration) :
			logger_(configuration.logger),
			outputType_(ListOutputType::LOG)
		{
		}

		~List() override = default;

		static constexpr std::string_view NAME = "list";

		void Parse(, std::span<std::string_view> arguments) override
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
		}

		void Process() override
		{
			api::ListParameters parameters();

			api::ListResult result = api::List(parameters);

			ListParameters parameters(logger_.CreateLogger("list"));

			auto result = interface.List(parameters);

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
					case ListOutputType::LOG :
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
					case ListOutputType::JSON :
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
		std::unique_ptr<Runner> runner_;
		std::reference_wrapper<const log::Logger> logger_;

		ListOutputType outputType_;
		std::optional<std::filesystem::path> file_;
	};
}
