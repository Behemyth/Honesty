export module synodic.honesty.test:runner.default;

import std;
import :runner;

namespace synodic::honesty::test
{
	export class DefaultRunner final : public Runner
	{
	public:
		explicit DefaultRunner(log::Logger logger)
			: Runner(std::move(logger))
		{
		}

		static consteval std::string_view Name()
		{
			return "default";
		}

		void Run(const Requirements& requirements, const std::function_ref<void(const Requirements&)> function) override
		{
			try
			{
				function(requirements);
			}
			catch (AssertException)
			{
			}
		}
	};
}
