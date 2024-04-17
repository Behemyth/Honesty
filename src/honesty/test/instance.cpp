module synodic.honesty.test;

import std;
import :interface;

// A helper for variant overload deduction
template<typename... Ts>
struct Overload : Ts...
{
	using Ts::operator()...;
};

namespace synodic::honesty::test
{
	Instance::Configuration::Configuration() :
		defaultRunner("default"),
		defaultReporter("default")
	{
	}

	Instance::Instance(const Configuration& configuration, std::span<std::string_view> arguments) :
		logger_("instance"),
		parameters_(HelpParameters())
	{
		logger_.AddSink(&consoleSink_);

		std::span<Runner*> runners = GetRegistry().GetRunners();
		std::span<Reporter*> reporters = GetRegistry().GetReporters();

		Runner* defaultRunner = nullptr;
		Reporter* defaultReporter = nullptr;

		std::string_view defaultRunnerName = "default";
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
				runners,
				[&](const Runner* runner) -> bool
				{
					return runner->Name() == defaultRunnerName;
				});

			if (iterator != runners.end())
			{
				defaultRunner = *iterator;
			}
			else
			{
				throw std::invalid_argument("The default runner specified does not exist");
			}
		}

		// Find the default reporter
		{
			auto iterator = std::ranges::find_if(
				reporters,
				[&](const Reporter* reporter) -> bool
				{
					return reporter->Name() == defaultReporterName;
				});

			if (iterator != reporters.end())
			{
				defaultReporter = *iterator;
			}
			else
			{
				throw std::invalid_argument("The default reporter specified does not exist");
			}
		}

		if (arguments.empty())
		{
			parameters_ = ExecuteParameters(defaultRunner, defaultReporter);
			return;
		}

		if (arguments[0] == "list")
		{
			arguments = arguments.subspan(1);
			ListParameters parameters;

			if (std::ranges::contains(arguments, "--json"))
			{
				parameters.outputType = ListOutputType::JSON;
			}

			parameters_ = parameters;
			return;
		}

		parameters_ = ExecuteParameters(defaultRunner, defaultReporter);
	}

	void Instance::Execute() const
	{
		Interface::Configuration configuration;
		Interface interface(configuration);

		try
		{
			auto executor = Overload{
				[&](const HelpParameters& parameters)
				{
					auto result = interface.Help(parameters);
				},
				[&](const ExecuteParameters& parameters)
				{
					auto result = interface.Execute(parameters);
				},
				[&](const ListParameters& parameters)
				{
					auto result = interface.List(parameters);

					for (auto& testDescription: result.tests)
					{
						logger_.Info("{}", testDescription.name);
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
}
