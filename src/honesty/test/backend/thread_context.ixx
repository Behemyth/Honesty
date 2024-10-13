
export module synodic.honesty.test.backend:thread_context;

import std;
import synodic.honesty.log;

namespace synodic::honesty::test
{
	export class ThreadContext
	{
	public:
		explicit ThreadContext(log::Logger logger) :
			logger_(std::move(logger))
		{
		}

	private:
		log::Scatter sink_;
		log::Logger logger_;
	};
}
