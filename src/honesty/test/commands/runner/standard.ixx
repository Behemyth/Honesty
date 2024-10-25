export module synodic.honesty.test.commands:runner.standard;

import std;
import synodic.honesty.test;

import function_ref;

namespace synodic::honesty::test
{
	export class StandardRunner final : public Runner
	{
	public:
		explicit StandardRunner(const log::Logger& logger) :
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

		Generator Run(Fixture& fixture, const std::function_ref<Generator(Fixture&)> function) override
		{
			return function(fixture);
		}

		Generator Run(const std::function_ref<Generator()> function) override
		{
			return function();
		}

	};
}
