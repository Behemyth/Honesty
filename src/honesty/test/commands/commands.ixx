
export module synodic.honesty.test.commands;

import std;

import synodic.honesty.test.types;
import synodic.honesty.test.backend;

import synodic.honesty.log;
import synodic.honesty.utility;

import :types;
import :execute;
export import :list;

import :reporter.standard;
import :reporter.compact;
import :runner.standard;

namespace
{
	// A helper for variant overload deduction
	template<typename... Ts>
	struct Overload : Ts...
	{
		using Ts::operator()...;
	};

	template<class T, typename U>
	struct TypeInVariant;

	template<class T, typename... Ts>
	struct TypeInVariant<T, std::variant<Ts...>> : std::bool_constant<(std::is_same_v<T, Ts> || ...)>
	{
	};

	template<typename T, typename V>
	concept type_in_variant = TypeInVariant<T, V>::value;
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
			explicit Configuration(const std::string_view name, log::Sink* sink, const std::size_t threadCount = 1) :
				defaultRunnerName("default"),
				defaultReporterName("default"),
				sink(sink),
				name(name),
				threadCount(threadCount)
			{
				if (threadCount == 0)
				{
					this->threadCount = std::thread::hardware_concurrency();
				}
			}

			std::string_view defaultRunnerName;
			std::string_view defaultReporterName;

			log::Sink* sink;
			std::string_view name;

			std::size_t threadCount;
		};

		// Resolve all input into immediately executable state ready for the 'Execute' function
		Instance(const Configuration& configuration, std::span<std::string_view> arguments) :
			applicationName_(std::filesystem::path(arguments.front()).stem().generic_string()),
			sink_(configuration.sink),
			appLogger_(log::RootLogger().CreateLogger(configuration.name)),
			command_(std::monostate {})
		{
			appLogger_.SetSink(sink_);

			command::Configuration commandConfiguration = ResolveConfiguration(configuration);
			appLogger_.Debug("Finding command for `{}` application", commandConfiguration.applicationName);

			// If the executable and at least one argument is given we can look for subcommands
			if (arguments.size() >= 2)
			{
				// Check for an option or flag
				if (std::string_view possibleSubCommand = arguments[1];
					not possibleSubCommand.starts_with("-") and not possibleSubCommand.starts_with("--"))
				{
					SearchSubCommand(possibleSubCommand, commandConfiguration);
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
					auto subExecutor = Overload {[this, &configuration, &arguments](auto& subCommand)
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

		template<type_in_variant<SubType> Cmd>
		[[no_discard]] std::optional<const typename Cmd::Data> CommandData()
		{
			if (const auto* subtype = std::get_if<SubType>(&command_))
			{
				if (const Cmd* command = std::get_if<Cmd>(subtype))
				{
					return command->GetData();
				}
			}
			return {};
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
				const log::TextStyle style(log::TerminalColor::RED);
				std::string message = log::format(style, "Internal Exception: {}", e.what());

				appLogger_.Error("{}", message);

				// TODO: Replace with a return code
				std::exit(134);
			}
		}

	private:
		template<std::size_t Index = 0>
		void SearchSubCommand(
			const std::string_view possibleSubCommand,
			const command::Configuration& commandConfiguration)
		{
			constexpr std::size_t size = std::variant_size_v<SubType>;

			if constexpr (Index < size)
			{
				using CommandType = std::variant_alternative_t<Index, SubType>;

				if (const std::string_view commandName = CommandType::NAME; commandName == possibleSubCommand)
				{
					command_.emplace<SubType>(std::in_place_type<CommandType>, commandConfiguration);
				}
				else
				{
					SearchSubCommand<Index + 1>(possibleSubCommand, commandConfiguration);
				}
			}
		}

		/**
		 * @brief Resolve the configuration into a command configuration to be passed to the instantiated command.
		 * @return The resolved command configuration.
		 */
		[[nodiscard]] command::Configuration ResolveConfiguration(const Configuration& configuration) const
		{
			return command::Configuration(applicationName_, appLogger_);
		}

		/***
		 * @brief Resolve the parse result into the selected runner and reporters and set the instance up for
		 * execution.
		 * @param configuration The configuration to use.
		 * @param parseResult The parsing result.
		 */
		void ResolveParseResult(const Configuration& configuration, const command::ParseResult& parseResult)
		{
			// Register our default runners and reporters
			{
				static ReporterRegistrar<StandardReporter> defaultReporterRegistrar;
				static ReporterRegistrar<CompactReporter> compactReporterRegistrar;

				static RunnerRegistrar<StandardRunner> defaultRunnerRegistrar;
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

					runner_ = registry->Create(appLogger_);
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
					reporters_.push_back(registry->Create(appLogger_));
				}
				else
				{
					throw std::invalid_argument("The reporters specified does not exist");
				}
			}
		}

		std::string applicationName_;

		log::Sink* sink_;
		log::Logger appLogger_; // The shared logger for the application

		TopType command_;

		std::unique_ptr<Runner> runner_;
		std::vector<std::unique_ptr<Reporter>> reporters_;
	};
}
