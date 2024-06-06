
export module synodic.honesty.test:instance;

import std;
import :interface;
import :context;
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
	public:
		struct Configuration
		{
			Configuration() :
				defaultRunner("default"),
				defaultReporter("default") {};

			std::string_view defaultRunner;	 // The default runner to use
			std::string_view defaultReporter;  // The default reporter to use
		};

		struct HelpContext
		{
			HelpContext() = default;
		};

		struct ExecuteContext
		{
			ExecuteContext(std::unique_ptr<Runner> runner, std::unique_ptr<Reporter> reporter) :
				runner(std::move(runner)),
				reporter(std::move(reporter))
			{
			}

			std::unique_ptr<Runner> runner;
			std::unique_ptr<Reporter> reporter;
		};

		struct ListContext
		{
			ListContext(std::unique_ptr<Runner> runner, log::Logger logger) :
				runner(std::move(runner)),
				logger(std::move(logger)),
				outputType(ListOutputType::LOG)
			{
			}

			std::unique_ptr<Runner> runner;
			log::Logger logger;

			ListOutputType outputType;
			std::optional<std::filesystem::path> file;
		};

		// Resolve all input into immediately executable state ready for the 'Execute' function
		Instance(const Configuration& configuration, std::span<std::string_view> arguments) :
			logger_(log::RootLogger().CreateLogger("instance")),
			parameters_(HelpContext())

		{
			logger_.SetSink(&consoleSink_);

			// Register our default runners and reporters
			{
				static ReporterRegistrar<DefaultReporter> defaultReporterRegistrar;
				static ReporterRegistrar<CompactReporter> compactReporterRegistrar;

				static RunnerRegistrar<DefaultRunner> defaultRunnerRegistrar;
			}

			// Gather our user defined runners and reporters
			std::span<RunnerRegistry*> runnerRegistrars		= RunnerRegistry::Registrars();
			std::span<ReporterRegistry*> reporterRegistrars = ReporterRegistry::Registrars();

			// TODO: Allocate memory
			std::unique_ptr<Runner> defaultRunner;
			std::unique_ptr<Reporter> defaultReporter;

			std::string_view defaultRunnerName =
				configuration.defaultRunner.empty() ? "default" : configuration.defaultRunner;
			std::string_view defaultReporterName =
				configuration.defaultReporter.empty() ? "default" : configuration.defaultReporter;

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
					const RunnerRegistry* registry = *iterator;
					defaultRunner				   = registry->Create(logger_.CreateLogger(registry->Name()));
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
					const ReporterRegistry* registry = *iterator;
					defaultReporter					 = registry->Create(logger_.CreateLogger(registry->Name()));
				}
				else
				{
					throw std::invalid_argument("The reporter specified does not exist");
				}
			}

			// Handle the command setup
			{
				if (arguments.empty())
				{
					parameters_ = ExecuteContext(std::move(defaultRunner), std::move(defaultReporter));
					return;
				}

				if (arguments[0] == "list")
				{
					arguments = arguments.subspan(1);

					ListContext parameters((std::move(defaultRunner)), logger_.CreateLogger("list"));

					if (std::ranges::contains(arguments, "--json"))
					{
						parameters.outputType = ListOutputType::JSON;
					}

					if (auto itr = std::ranges::find(arguments, "--file"); itr != arguments.end())
					{
						if(++itr == arguments.end())
						{
							throw std::invalid_argument("You must give a file name when using the '--file' option");
						}

						parameters.file = *itr;
					}

					parameters_ = std::move(parameters);
					return;
				}

				parameters_ = ExecuteContext(std::move(defaultRunner), std::move(defaultReporter));
			}
		}

		const HelpContext* GetHelpContext() const
		{
			return std::get_if<HelpContext>(&parameters_);
		}

		const ExecuteContext* GetExecuteContext() const
		{
			return std::get_if<ExecuteContext>(&parameters_);
		}

		const ListContext* GetListContext() const
		{
			return std::get_if<ListContext>(&parameters_);
		}

		void Execute()
		{
			Interface::Configuration configuration;
			Interface interface(configuration);

			try
			{
				auto executor = Overload {
					[&](const HelpContext& context)
					{
						constexpr HelpParameters parameters;
						auto result = interface.Help(parameters);
					},
					[&](const ExecuteContext& context)
					{
						std::ranges::single_view reporters {context.reporter.get()};

						// Before start executing, we need to set up the current thread's context
						Context commandContext = Context(*context.runner.get(), reporters);
						ExecuteParameters parameters(commandContext);

						auto result = interface.Execute(parameters);
					},
					[&](const ListContext& context)
					{
						ListParameters parameters(logger_.CreateLogger("list"));

						auto result = interface.List(parameters);

						switch (context.outputType)
						{
							case ListOutputType::LOG :
							{
								for (auto& testDescription: result.tests)
								{
									logger_.Info("{}", testDescription.name);
								}
								break;
							}
							case ListOutputType::JSON :
							{
								break;
							}
						}
					},
				};

				std::visit(executor, parameters_);
			}
			catch (const std::invalid_argument& exception)
			{
				logger_.Error("Error: {0}", exception.what());
			}
		}

	private:
		log::Console consoleSink_;
		log::Logger logger_;

		// Our list of top level commands and the parameters that go with them
		std::variant<HelpContext, ExecuteContext, ListContext> parameters_;
	};

}
