export module synodic.honesty.test:runner.default;

import std;
import :runner;

namespace synodic::honesty::test
{
	export class DefaultRunner final : public Runner
	{
	public:
		consteval explicit DefaultRunner(std::string_view name);

		void Run(std::function_ref<void()> function) override;
	};

	consteval DefaultRunner::DefaultRunner(std::string_view name) :
		Runner(name)
	{
	}
}
