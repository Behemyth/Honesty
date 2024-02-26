export module synodic.honesty.test:runner.local;

import std;
import :runner;

namespace synodic::honesty::test::runner
{
	export class Local : public Runner
	{
	public:
		explicit Local()
		{
		}

		void Run(const Events& events) override;
		void Submit(const SuiteData* data) override;
		void Submit(std::span<const SuiteData* const> data) override;

	private:
		std::vector<const SuiteData*> suites_;
	};
}
