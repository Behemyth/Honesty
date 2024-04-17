export module synodic.honesty.test:runner.local;

import std;
import :runner;

namespace synodic::honesty::test
{
	export class Local final : public Runner
	{
	public:
		consteval explicit Local(std::string_view name);

		void Run(Broadcast& broadcaster) override;
		void Submit(const SuiteData* data) override;
		void Submit(std::span<const SuiteData* const> data) override;

	private:
		std::array<const SuiteData*, 30> suitesData_;
		std::span<const SuiteData*> suites_;
	};

	consteval Local::Local(std::string_view name) :
		Runner(name),
		suitesData_(),
		suites_(suitesData_.data(), 0)
	{
	}
}
