
export module synodic.honesty.test:instance;

import std;
import :interface;
import :context;

// A helper for variant overload deduction
template<typename... Ts>
struct Overload : Ts...
{
	using Ts::operator()...;
};

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

		// Resolve all input into immediately executable state ready for the 'Execute' function
		Instance(const Configuration& configuration, std::span<std::string_view> arguments) :
			logger_(log::RootLogger().CreateLogger("instance")),
			parameters_(HelpParameters())
		{
			// Gather our runners and reporters
			const std::span<RunnerRegistry*> runnerRegistrars	  = RunnerRegistry::Registrars();
			const std::span<ReporterRegistry*> reporterRegistrars = ReporterRegistry::Registrars();

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
					parameters_ = ExecuteParameters(defaultRunner.get(), defaultReporter.get());
					return;
				}

				if (arguments[0] == "list")
				{
					arguments = arguments.subspan(1);

					ListParameters parameters(defaultRunner.get(), logger_.CreateLogger("list"));

					if (std::ranges::contains(arguments, "--json"))
					{
						parameters.outputType = ListOutputType::JSON;
					}

					parameters_ = std::move(parameters);
					return;
				}

				parameters_ = ExecuteParameters(defaultRunner.get(), defaultReporter.get());
			}
		}

		void Execute()
		{
			Interface::Configuration configuration;
			Interface interface(configuration);

			try
			{
				auto executor = Overload {
					[&](const HelpParameters& parameters)
					{
						auto result = interface.Help(parameters);
					},
					[&](const ExecuteParameters& parameters)
					{
						auto result = interface.Execute(parameters);
					},
					[&](ListParameters& parameters)
					{
						auto result = interface.List(parameters);

						switch (parameters.outputType)
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
		std::variant<HelpParameters, ExecuteParameters, ListParameters> parameters_;
	};

}
