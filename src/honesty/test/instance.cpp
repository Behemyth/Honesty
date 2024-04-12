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

	Instance::Instance(const Configuration& configuration, std::span<std::string_view> arguments)
	{
		if (std::ranges::contains(arguments, "--list-tests"))
		{
			parameters_ = ListParameters();
		}
		else
		{
			parameters_ = ExecuteParameters();
		}
	}

	void Instance::Execute() const
	{
		try
		{
			Interface::Configuration configuration;

			Interface interface(configuration);
			auto executor = Overload{
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
