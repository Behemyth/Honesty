export module synodic.honesty.test.runner:local;

import std;
import synodic.honesty.test.backend;

export namespace synodic::honesty::runner
{
	class Local  : public Runner
	{
	public:
		Local() :
			Runner("default")
		{
		}

		void Run() override;
		void Submit(SuiteData data) override;
		void Submit(std::vector<SuiteData> data) override;

	private:
		std::vector<SuiteData> suites_;
	};

	//Suite suite("test Suite", testSuite);
	//bool registered = suite.Register();
}

