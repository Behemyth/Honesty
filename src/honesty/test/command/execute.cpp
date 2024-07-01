module synodic.honesty.test:command.execute;

import std;

import :api;
import :command.types;

namespace synodic::honesty::test::command
{
	class Execute final : public Command
	{
	public:
		explicit Execute(const Configuration& configuration) :
			configuredRunnerRegistry_(configuration.configuredRunnerRegistry),
			configuredReporterRegistry_(configuration.configuredReporterRegistry),
			logger_(configuration.logger)
		{
		}

		~Execute() override = default;

		void Parse(std::span<std::string_view> arguments) override
		{
			if (auto itr = std::ranges::find(arguments, "--filter"); itr != arguments.end())
			{
				if (++itr == arguments.end())
				{
					throw std::invalid_argument("You must give a filter when using the '--filter' option");
				}

				filter_ = *itr;
			}
		}

		void Process() override
		{
			const api::ExecuteParameters
				parameters(applicationName_, filter_, configuredRunnerRegistry_, configuredReporterRegistry_, logger_);
			const api::ExecuteResult result = api::Execute(parameters);

			if (not result.success)
			{
				// TODO: Replace with a return code
				std::exit(134);
			}
		}

	private:
		std::string applicationName_;
		std::string filter_;

		std::reference_wrapper<const RunnerRegistry> configuredRunnerRegistry_;
		std::reference_wrapper<const ReporterRegistry> configuredReporterRegistry_;

		std::reference_wrapper<const log::Logger> logger_;
	};
}
