module synodic.honesty.test;

import std;
import :interface;

namespace synodic::honesty::test
{
	Instance::Instance(const Configuration& configuration, std::span<std::string_view> arguments)
	{
		Interface::Command command;

		if (std::ranges::contains(arguments, "--list-tests"))
		{
			command = Interface::Command::LIST;
		}
		else
		{
			command = Interface::Command::EXECUTE;
		}

		command_ = command;
	}

	void Instance::Execute() const
	{
		try
		{
			Interface interface;
			switch (command_)
			{
				case Interface::Command::EXECUTE:
					interface.Execute(configuration_);
					break;
				case Interface::Command::LIST:
					interface.List(configuration_);
					break;
			}
		}
		catch (const std::invalid_argument& exception)
		{
			std::println("Error: {0}", exception.what());
		}
	}
}
