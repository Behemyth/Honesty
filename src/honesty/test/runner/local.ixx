export module synodic.honesty.test:runner.local;

import std;
import :runner;

namespace synodic::honesty::test
{
	export class Local final : public Runner
	{
	public:
		consteval explicit Local(std::string_view name);

		void Run(std::span<Set> sets, std::function_ref<RunnerContext&()> generateContext) override;
	};

	consteval Local::Local(std::string_view name) :
		Runner(name)
	{
	}
}
