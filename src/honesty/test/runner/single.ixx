export module synodic.honesty.test.runner:single;

import std;
import synodic.honesty.test;

export namespace synodic::honesty::runner
{
	class single_threaded
	{
	public:
		void run(std::span<std::move_only_function<Generator()>> suites) const;

	private:
	};
}
