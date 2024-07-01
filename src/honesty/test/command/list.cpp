module synodic.honesty.test:command.list;

import std;

import :api;
import :command.types;

namespace synodic::honesty::test::command
{
	struct Context
	{
		Context(std::unique_ptr<Runner> runner, const log::Logger& logger) :
			runner(std::move(runner)),
			logger(logger),
			outputType(ListOutputType::LOG)
		{
		}

		std::unique_ptr<Runner> runner;
		std::reference_wrapper<const log::Logger> logger;

		ListOutputType outputType;
		std::optional<std::filesystem::path> file;
	};

	class List : public Command
	{
	public:
		~List() override = default;

		static constexpr std::string_view NAME = "list";

		void Parse(std::span<std::string_view> arguments) override
		{
			arguments = arguments.subspan(1);

			ListContext parameters((std::move(defaultRunner)), logger_.CreateLogger("list"));

			if (std::ranges::contains(arguments, "--json"))
			{
				parameters.outputType = ListOutputType::JSON;
			}

			if (auto itr = std::ranges::find(arguments, "--file"); itr != arguments.end())
			{
				if (++itr == arguments.end())
				{
					throw std::invalid_argument("You must give a file name when using the '--file' option");
				}

				parameters.file = absolute(std::filesystem::current_path() / *itr);

				if (not parameters.file->has_filename())
				{
					throw std::invalid_argument(
						"You must give a valid file name when using the '--file' option");
				}
			}

			parameters_ = std::move(parameters);
			return;
		}

		void Process() override
		{
		}

	private:
		std::unique_ptr<Runner> runner_;
		std::reference_wrapper<const log::Logger> logger_;

		ListOutputType outputType_;
		std::optional<std::filesystem::path> file_;
	};
}
