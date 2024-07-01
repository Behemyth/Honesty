
export module synodic.honesty.test:command;

import std;
import synodic.honesty.utility;

import :command.execute;
import :command.list;

import :reporter.default;
import :reporter.compact;
import :runner.default;

namespace
{
	// A helper for variant overload deduction
	template<typename... Ts>
	struct Overload : Ts...
	{
		using Ts::operator()...;
	};

	// Helper for iterating over a range of indices at compile-time
	template<std::size_t Start, std::size_t End, std::size_t Inc, class Fn>
	constexpr void ConstantFor(Fn&& function)
	{
		if constexpr (Start < End)
		{
			function(std::integral_constant<decltype(Start), Start>());
			constexpr_for<Start + Inc, End, Inc>(function);
		}
	}
}

namespace synodic::honesty::test
{
	/**
	 * @brief The proper way to interact with Honesty via a 'main' function. A class in two parts. First, it resolves
	 *	the input configuration into commands and parameters that will be executed. Then, when executed, it sets up the
	 *	running execution state for each thread running the commands.
	 */
	export class Instance
	{
		// Subcommand storage
		using SubType = std::variant<command::List>;

		// Top level command storage
		using TopType = std::variant<std::monostate, command::Execute, SubType>;

	public:
		struct Configuration
		{
			explicit Configuration(std::string_view name, log::Sink* sink) :
				defaultRunnerName("default"),
				defaultReporterName("default"),
				sink(sink),
				name(name)
			{
			}

			std::string_view defaultRunnerName;
			std::string_view defaultReporterName;

			log::Sink* sink;
			std::string_view name;
		};

		// Resolve all input into immediately executable state ready for the 'Execute' function
		Instance(const Configuration& configuration, std::span<std::string_view> arguments) :
			sink_(configuration.sink),
			logger_(log::RootLogger().CreateLogger(configuration.name)),
			command_(std::monostate {}),
			applicationName_(std::filesystem::path(arguments.front()).stem().generic_string())

		{
			logger_.SetSink(sink_);

			command::Configuration commandConfiguration = ResolveConfiguration(configuration);

			// If the executable and at least one argument is given we can look for subcommands
			if (arguments.size() >= 2)
			{
				// Check for an option or flag
				if (std::string_view possibleSubCommand = arguments[1];
					not possibleSubCommand.starts_with("-") and not possibleSubCommand.starts_with("--"))
				{
					// Look for a matching subcommand
					auto apply = [this, &commandConfiguration, possibleSubCommand](auto index) consteval -> void
					{
						using CommandType = std::variant_alternative_t<index, SubType>;

						static_assert(command<CommandType>, "Not a valid command type");

						if (const std::string_view commandName = CommandType::NAME; commandName == possibleSubCommand)
						{
							command_.emplace<SubType>(CommandType(commandConfiguration));
						}
					};

					ConstantFor<2, std::variant_size_v<SubType>, 1>(apply);
				}
			}

			// If we have mono-state we know there is no subcommand and initialize the execute command
			if (std::holds_alternative<std::monostate>(command_))
			{
				command_.emplace<command::Execute>(commandConfiguration);

				arguments = arguments.subspan(1);
			}
			else
			{
				arguments = arguments.subspan(2);
			}

			// Overload the visitor and make use of the pure-virtual interface for deduction
			auto executor = Overload {[&](auto& command)
									  {
										  command.Parse(arguments);
									  }};

			std::visit(executor, command_);
		}

		void Execute()
		{
			try
			{
				const Interface::Configuration configuration(applicationName_);
				Interface api(configuration);

				// Overload the visitor and make use of the pure-virtual interface for deduction
				auto executor = Overload {[&](auto& command)
										  {
											  command.Process();
										  }};

				std::visit(executor, command_);
			}
			catch (std::exception& e)
			{
				logger_.Error("Internal Exception: {}", e.what());

				// TODO: Replace with a return code
				std::exit(134);
			}
		}

	private:
		/**
		 * @brief Resolve the configuration into a command configuration.
		 * @return The resolved command configuration.
		 */
		[[nodiscard]] command::Configuration ResolveConfiguration(const Configuration& configuration) const
		{
			// Register our default runners and reporters
			{
				static ReporterRegistrar<DefaultReporter> defaultReporterRegistrar;
				static ReporterRegistrar<CompactReporter> compactReporterRegistrar;

				static RunnerRegistrar<DefaultRunner> defaultRunnerRegistrar;
			}

			// Gather our user defined runners and reporters
			const std::span<RunnerRegistry*> runnerRegistrars	  = RunnerRegistry::Registrars();
			const std::span<ReporterRegistry*> reporterRegistrars = ReporterRegistry::Registrars();

			// TODO: Allocate memory
			const RunnerRegistry* selectedRunner;
			const ReporterRegistry* selectedReporter;

			const std::string_view defaultRunnerName =
				configuration.defaultRunnerName.empty() ? "default" : configuration.defaultRunnerName;
			const std::string_view defaultReporterName =
				configuration.defaultReporterName.empty() ? "default" : configuration.defaultReporterName;

			// Find the runner
			{
				const auto iterator = std::ranges::find_if(
					runnerRegistrars,
					[&](const RunnerRegistry* registry) -> bool
					{
						return registry->Name() == defaultRunnerName;
					});

				if (iterator != runnerRegistrars.end())
				{
					selectedRunner = *iterator;
				}
				else
				{
					throw std::invalid_argument("The runner specified does not exist");
				}
			}

			// Find the reporter
			{
				const auto iterator = std::ranges::find_if(
					reporterRegistrars,
					[&](const ReporterRegistry* registry) -> bool
					{
						return registry->Name() == defaultReporterName;
					});

				if (iterator != reporterRegistrars.end())
				{
					selectedReporter = *iterator;
				}
				else
				{
					throw std::invalid_argument("The reporter specified does not exist");
				}
			}

			return command::Configuration(
				*selectedRunner,
				*selectedReporter,
				runnerRegistrars,
				reporterRegistrars,
				logger_);
		}

		log::Sink* sink_;
		log::Logger logger_;

		TopType command_;

		std::string applicationName_;
	};

}
