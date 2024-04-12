module synodic.honesty.test;

import std;
import :interface;

namespace synodic::honesty::test
{
	template<typename... Ts>
	struct Overload : Ts...
	{
		using Ts::operator()...;
	};

	Instance::Configuration::Configuration() :
		defaultRunner("default"),
		defaultReporter("default")
	{
	}

	Instance::Instance(const Configuration& configuration, std::span<std::string_view> arguments) :
		parameters_(HelpParameters())
	{
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

		// TODO: Proper argument parsing
		if (std::ranges::contains(arguments, "--list-tests"))
		{
			parameters_ = ListParameters();
		}
		else
		{
			parameters_ = ExecuteParameters(defaultRunner, defaultReporter);
		}
	}

	void Instance::Execute() const
	{
		try
		{
			Interface::Configuration configuration;

			Interface interface(configuration);
			auto executor = Overload{
				[&](const HelpParameters& parameters)
				{
					interface.Help(parameters);
				},
				[&](const ExecuteParameters& parameters)
				{
					interface.Execute(parameters);
				},
				[&](const ListParameters& parameters)
				{
					interface.List(parameters);
				},
			};

			std::visit(executor, parameters_);
		}
		catch (const std::invalid_argument& exception)
		{
			std::println("Error: {0}", exception.what());
		}
	}
}
