module synodic.honesty.test;

import std;
import :interface;

namespace synodic::honesty::test
{
	Instance::Instance(Configuration& configuration, std::span<std::string_view> arguments)
	{
		try
		{
			Configuration configuration;
			Instance instance(configuration, arguments);

			if (std::ranges::contains(arguments, "list-tests"))
			{
				parameter::List parameters;

				interface_.List(parameters);
			}
			else
			{
				parameter::Execute parameters;
				interface_.Execute(parameters);
			}
		}
		catch (const std::invalid_argument& exception)
		{
			std::println("Error: {0}", exception.what());
		}
	}
}
