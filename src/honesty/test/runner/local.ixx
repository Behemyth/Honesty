export module synodic.honesty.test.runner:local;

import std;
import synodic.honesty.test.backend;

export namespace synodic::honesty::runner
{
	class Local : public Runner
	{
	public:
		Local() :
			Runner("default")
		{
		}

		void Run() override;
		void Submit(const SuiteData* data) override;
		void Submit(std::span<const SuiteData* const> data) override;

	private:
		std::vector<const SuiteData*> suites_;
	};

	// Suite suite("test Suite", testSuite);
	// bool registered = suite.Register();
}
