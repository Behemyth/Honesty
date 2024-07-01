module synodic.honesty.test:command.execute;

import std;

import :api;
import :command.types;

namespace synodic::honesty::test::command
{
	struct Context
	{
		Context(std::unique_ptr<Runner> runner, std::unique_ptr<Reporter> reporter, const std::string& filter) :
			runner(std::move(runner)),
			reporter(std::move(reporter)),
			filter(filter)
		{
		}

		std::unique_ptr<Runner> runner;
		std::unique_ptr<Reporter> reporter;

		std::string filter;
	};

	class Execute : public Command
	{
	public:
		Execute();
		~Execute() override = default;

		void Parse(std::span<std::string_view> arguments) override
		{
			ExecuteContext parameters(std::move(defaultRunner), std::move(defaultReporter), "");

			if (auto itr = std::ranges::find(arguments, "--filter"); itr != arguments.end())
			{
				if (++itr == arguments.end())
				{
					throw std::invalid_argument("You must give a filter when using the '--filter' option");
				}

				parameters.filter = *itr;
			}

			commands_ = std::move(parameters);
			return;
		}

		void Process() override
		{
		}

	private:
		std::unique_ptr<Runner> runner_;
		std::unique_ptr<Reporter> reporter_;

		std::string filter_;
	};
}
