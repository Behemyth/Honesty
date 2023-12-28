export module synodic.honesty.test.runner:single;

import std;
import synodic.honesty.test.backend;

export namespace synodic::honesty::runner
{
	class single_threaded
	{
	public:
		void run(std::span<const suite_data> suites) const;

	private:
	};
}
