module synodic.honesty.test:command.types;

import std;

namespace synodic::honesty::log
{
	class Logger;
}

namespace synodic::honesty::test
{
	class ReporterRegistry;
	class RunnerRegistry;
}

namespace synodic::honesty::test::command
{
	struct Configuration
	{
		Configuration(
			std::string_view applicationName,
			const RunnerRegistry& configuredRunnerRegistry,
			const ReporterRegistry& configuredReporterRegistry,
			const std::span<RunnerRegistry*> runnerRegistrars,
			const std::span<ReporterRegistry*> reporterRegistrars,
			const log::Logger& logger) :
			applicationName(applicationName),
			configuredRunnerRegistry(configuredRunnerRegistry),
			configuredReporterRegistry(configuredReporterRegistry),
			runnerRegistrars(runnerRegistrars),
			reporterRegistrars(reporterRegistrars),
			logger(logger)
		{
		}

		std::string_view applicationName;

		std::reference_wrapper<const RunnerRegistry> configuredRunnerRegistry;
		std::reference_wrapper<const ReporterRegistry> configuredReporterRegistry;

		std::span<RunnerRegistry*> runnerRegistrars;
		std::span<ReporterRegistry*> reporterRegistrars;

		std::reference_wrapper<const log::Logger> logger;
	};

	class Command
	{
	public:
		constexpr Command() = default;
		virtual ~Command() = default;

		Command(const Command& other) = delete;
		Command(Command&& other) noexcept = delete;
		auto operator=(const Command& other) -> Command& = delete;
		auto operator=(Command&& other) noexcept -> Command& = delete;

		virtual void Parse(std::span<std::string_view> arguments) = 0;
		virtual void Process() = 0;

	private:
	};

	template<typename T>
	concept command = requires(const Configuration& config)
	{
		// Constructable from a Configuration
		T(config);

		// The command must have a name, and it must be a string_view
		{
			T::NAME
		} -> std::same_as<std::string_view>;

		// The name must be constant
		{
			std::bool_constant<T::NAME>()
		} -> std::same_as<std::true_type>;

		// Must be derived from the Command class
		std::derived_from<T, Command>;
	};
}
