export module synodic.honesty.test.runner:single;

import std;
import synodic.honesty.test.backend;

export namespace synodic::honesty::runner
{
	class single_threaded  : public Runner
	{
	public:
		single_threaded() :
			Runner("default")
		{
		}

		void Run() override;
		void Submit(suite_data data) override;
		void Submit(std::vector<suite_data> data) override;

	private:
		std::vector<suite_data> suites_;
	};
}
