
export module synodic.honesty.test.backend:runner;

import :test;
import :suite;
import :reporter;
import std;

namespace synodic::honesty
{

	/**
	 * \brief The type of runner can be selected by the user when invoking tests, for example, via the command line
	 * interface. Additionally, the user can specify a specific runner for a specific test suite. Test suites must be
	 * registered with runtime registration to avoid having every translation unit recompile when mo. With
	 * these two constraints its clear that the Runner interface must be runtime polymorphic.
	 */
	export class Runner
	{
	public:
		/**
		 * @brief
		 */
		explicit constexpr Runner();

		virtual ~Runner() = default;

		virtual void Run(const Events& events) = 0;

		virtual void Submit(const SuiteData* data)					= 0;
		virtual void Submit(std::span<const SuiteData* const> data) = 0;
	};

	constexpr Runner::Runner()
	{
	}
}
