export module synodic.honesty.benchmark:context;

namespace synodic::honesty::benchmark
{
	/**
	 *	@brief Base class for all internal benchmarking state
	 */
	class Context
	{
	};

	/**
	 *	@brief Context for iterative state
	 */
	export class RegressionContext : public Context
	{
	public:

	private:
	};

	/**
	 *	@brief Context for timed state
	 */
	export class TimedContext : public Context
	{
	public:

	private:
	};
}
