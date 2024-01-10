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
		void Submit(suite_data data) override;
		void Submit(std::vector<suite_data> data) override;

	private:
		std::vector<suite_data> suites_;
	};

	//Suite suite("test Suite", testSuite);
	//bool registered = suite.Register();
}

