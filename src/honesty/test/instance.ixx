
export module synodic.honesty.test:instance;

import std;
import :interface;
import :context;
import :reporter.default;
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
		};

		// Resolve all input into immediately executable state ready for the 'Execute' function
		Instance(const Configuration& configuration, std::span<std::string_view> arguments) :
			logger_(log::RootLogger().CreateLogger("instance")),
			parameters_(HelpContext())

		{
			logger_.SetSink(&consoleSink_);

			// Manually add the built-in runners and reporters
			ReporterRegistrar<DefaultReporter> defaultReporterRegistrar;
			RunnerRegistrar<DefaultRunner> defaultRunnerRegistrar;

			// Gather our user defined runners and reporters
			std::span<RunnerRegistry*> runnerRegistrars		= RunnerRegistry::Registrars();
			std::span<ReporterRegistry*> reporterRegistrars = ReporterRegistry::Registrars();

			// TODO: Allocate memory
			std::unique_ptr<Runner> defaultRunner;
			std::unique_ptr<Reporter> defaultReporter;

			std::string_view defaultRunnerName	 = "default";
			std::string_view defaultReporterName = "default";

			// Override the default runner and reporter if the input configuration is empty
			{
				if (configuration.defaultRunner.empty())
				{
					defaultRunnerName = configuration.defaultRunner;
				}

				if (configuration.defaultReporter.empty())
				{
					defaultReporterName = configuration.defaultReporter;
				}
			}

			// Find the default runner
			{
				auto iterator = std::ranges::find_if(
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
					throw std::invalid_argument("The default runner specified does not exist");
				}
			}

			// Find the default reporter
			{
				auto iterator = std::ranges::find_if(
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
					throw std::invalid_argument("The default reporter specified does not exist");
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

					parameters_ = std::move(parameters);
					return;
				}

				parameters_ = ExecuteContext(std::move(defaultRunner), std::move(defaultReporter));
			}
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
						ThreadContext& threadContext = GetThreadContext();

						ExecuteParameters parameters(context.runner.get(), context.reporter.get());
						std::ranges::single_view reporters {parameters.reporter};

						// Before start executing, we need to set up the current thread's context
						threadContext = ThreadContext(*parameters.runner, reporters);

						auto result = interface.Execute(parameters);
					},
					[&](const ListContext& context)
					{
						ThreadContext& threadContext = GetThreadContext();

						ListReporterParameters listReporterParameters;
						listReporterParameters.outputType = context.outputType;

						ListReporter reporter(listReporterParameters, logger_.CreateLogger("reporter"));

						ListParameters parameters(context.runner.get(), &reporter, logger_.CreateLogger("list"));
						std::ranges::single_view reporters {parameters.reporter};

						// Before start executing, we need to set up the current thread's context
						threadContext = ThreadContext(*parameters.runner, reporters);

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
