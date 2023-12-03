export module synodic.honesty.test.runner:single;

import std;

export namespace synodic::honesty::runner
{
	class single_threaded
	{
	public:
		void run(std::span<std::move_only_function<void()>> suites);
	private:
	};
}
