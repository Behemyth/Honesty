module synodic.honesty.test.commands:execute;

import std;

import synodic.honesty.test.api;
import :types;

namespace synodic::honesty::test::command
{
	class Execute final : public Command
	{
	public:
		explicit Execute(const Configuration& configuration) :
			logger_(configuration.logger)
		{
		}

		~Execute() override = default;

		auto Parse(std::span<std::string_view> arguments) -> ParseResult override
		{
			if (auto itr = std::ranges::find(arguments, "--filter"); itr != arguments.end())
			{
				if (++itr == arguments.end())
				{
					throw std::invalid_argument("You must give a filter when using the '--filter' option");
				}

				filter_ = *itr;
			}

			ParseResult result("", {});

			return result;
		}

		void Process(ProcessConfiguration& configuration) override
		{
			bool dryRun = false;
			const std::string header = std::format("{}\nDry run: {}", configuration.header, dryRun);

			const api::ExecuteParameters parameters(
				applicationName_,
				filter_,
				configuration.runner,
				configuration.reporters,
				dryRun,
				header,
				logger_);
			const api::ExecuteResult result = api::Execute(parameters);

			if (not result.success)
			{
				throw std::runtime_error("Test execution failed");
			}
		}

	private:
		std::string applicationName_;
		std::string filter_;

		std::reference_wrapper<const log::Logger> logger_;
	};
}
