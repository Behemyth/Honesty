
export module synodic.honesty.test.types:runner;

import std;

import inplace_vector;
import function_ref;

import :requirements;
import :reporter;
import :test;
import :fixture;

namespace synodic::honesty::test
{

	/**
	 * @brief The type of runner can be selected by the user when invoking tests, for example, via the command line
	 *	api. Additionally, the user can specify a specific runner for a specific test suite. Test suites must be
	 *	registered with runtime registration to avoid having every translation unit recompile. With
	 *	these two constraints the Runner api must be runtime polymorphic.
	 *	Handles the api to the test std::execution context.
	 */
	export class Runner
	{
	public:
		/**
		 * @brief
		 */
		explicit constexpr Runner(const log::Logger& logger) :
			logger_(logger)
		{
		}

		virtual ~Runner() = default;

		virtual void Run(const Requirements& requirements, std::function_ref<void(const Requirements&)> function) = 0;

		virtual Generator
			Run(const Requirements& requirements, std::function_ref<Generator(const Requirements&)> function) = 0;

		virtual Generator Run(Fixture& fixture, std::function_ref<Generator(Fixture&)> function) = 0;

		virtual Generator Run(std::function_ref<Generator()> function) = 0;

		const log::Logger& Logger() const
		{
			return logger_;
		}

	private:
		std::reference_wrapper<const log::Logger> logger_;
	};
}
