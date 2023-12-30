export module synodic.honesty.test.runner:single;

import std;
import synodic.honesty.test.backend;

export namespace synodic::honesty::runner
{
	class single_threaded
	{
	public:
		void run() const;
		void submit(suite_data data);

	private:
		std::vector<suite_data> suites_;
	};
}
