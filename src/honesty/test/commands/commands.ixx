
export module synodic.honesty.test.commands;

import std;

import synodic.honesty.test.types;
import synodic.honesty.test.backend;

import synodic.honesty.log;
import synodic.honesty.utility;

import :types;
import :execute;
import :list;

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
	consteval void ConstantFor(Fn&& function)
	{
		if constexpr (Start < End)
		{
			function(std::integral_constant<std::size_t, Start>());
			ConstantFor<Start + Inc, End, Inc>(function);
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
			applicationName_(std::filesystem::path(arguments.front()).stem().generic_string()),
			sink_(configuration.sink),
			logger_(log::RootLogger().CreateLogger(configuration.name)),
			command_(std::monostate {})

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
					auto apply = [this]<std::unsigned_integral T>(T) consteval -> void
					{
						/*constexpr std::size_t index = T;

						logger_.Info("Checking for subcommand: {}", index);*/
						//using CommandType = std::variant_alternative_t<index, SubType>;

						// static_assert(command<CommandType>, "Not a valid command type");

						// if (const std::string_view commandName = CommandType::NAME; commandName ==
						// possibleSubCommand)
						//{
						//	// command_.emplace<SubType>(CommandType(commandConfiguration));
						// }
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
			auto executor = Overload {
				[this, &configuration, &arguments](command::Execute& command)
				{
					const command::ParseResult result = command.Parse(arguments);
					ResolveParseResult(configuration, result);
				},
				[this, &configuration, &arguments](SubType& command)
				{
					auto subExecutor = Overload {[this, &configuration, &arguments](command::List& subCommand)
												 {
													 const command::ParseResult result = subCommand.Parse(arguments);
													 ResolveParseResult(configuration, result);
												 }};
					std::visit(subExecutor, command);
				},
				[](std::monostate)
				{

				}};

			std::visit(executor, command_);
		}

		void Execute()
		{
			try
			{
				// Overload the visitor and make use of the pure-virtual interface for deduction
				auto executor = Overload {
					[this](command::Execute& command)
					{
						command::ProcessConfiguration configuration(*runner_, reporters_);
						command.Process(configuration);
					},
					[this](SubType& command)
					{
						auto subExecutor =
							Overload {[this](auto& subCommand)
									  {
										  command::ProcessConfiguration configuration(*runner_, reporters_);
										  subCommand.Process(configuration);
									  }};
						std::visit(subExecutor, command);
					},
					[](std::monostate)
					{
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
		 * @brief Resolve the configuration into a command configuration to be passed to the instantiated command.
		 * @return The resolved command configuration.
		 */
		[[nodiscard]] command::Configuration ResolveConfiguration(const Configuration& configuration) const
		{
			return command::Configuration(applicationName_, logger_);
		}

		/***
		 * @brief Resolve the parse result into the selected runner and reporters and set the instance up for execution.
		 * @param configuration The configuration to use.
		 * @param parseResult The parsing result.
		 */
		void ResolveParseResult(const Configuration& configuration, const command::ParseResult& parseResult)
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

			std::string_view selectedRunner			   = parseResult.runnerOverride;
			std::vector<std::string> selectedReporters = parseResult.reporterOverrides;

			constexpr std::string_view defaultName = "default";

			if (selectedRunner.empty())
			{
				selectedRunner =
					configuration.defaultRunnerName.empty() ? defaultName : configuration.defaultRunnerName;
			}

			if (selectedReporters.empty())
			{
				if (configuration.defaultReporterName.empty())
				{
					selectedReporters.emplace_back(defaultName);
				}
				else
				{
					selectedReporters.emplace_back(configuration.defaultReporterName);
				}
			}

			// Find the runner
			{
				auto iterator = std::ranges::find_if(
					runnerRegistrars,
					[&](const RunnerRegistry* registry) -> bool
					{
						return registry->Name() == selectedRunner;
					});

				if (iterator != runnerRegistrars.end())
				{
					const RunnerRegistry* registry = *iterator;

					runner_ = registry->Create(logger_);
				}
				else
				{
					throw std::invalid_argument("The runner specified does not exist");
				}
			}

			// Find the reporters
			for (const std::string& reporterName: selectedReporters)
			{
				auto iterator = std::ranges::find_if(
					reporterRegistrars,
					[&](const ReporterRegistry* registry) -> bool
					{
						return registry->Name() == reporterName;
					});

				if (iterator != reporterRegistrars.end())
				{
					const ReporterRegistry* registry = *iterator;
					reporters_.push_back(registry->Create(logger_));
				}
				else
				{
					throw std::invalid_argument("The reporters specified does not exist");
				}
			}
		}

		std::string applicationName_;

		log::Sink* sink_;
		log::Logger logger_;

		TopType command_;

		std::unique_ptr<Runner> runner_;
		std::vector<std::unique_ptr<Reporter>> reporters_;
	};

}
