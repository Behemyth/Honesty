export module synodic.honesty.test.commands:runner.default;

import std;
import synodic.honesty.test;

namespace synodic::honesty::test
{
	export class DefaultRunner final : public Runner
	{
	public:
		explicit DefaultRunner(const log::Logger& logger) :
			Runner(logger)
		{
		}

		static consteval std::string_view Name()
		{
			return "default";
		}

		void Run(const Requirements& requirements, const std::function_ref<void(const Requirements&)> function) override
		{
			function(requirements);
		}

		Generator
			Run(const Requirements& requirements,
				const std::function_ref<Generator(const Requirements&)> function) override
		{
			return function(requirements);
		}
	};
}
