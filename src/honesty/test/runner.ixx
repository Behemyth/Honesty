
export module synodic.honesty.test:runner;

import :types;
import :reporter;
import std;

namespace synodic::honesty::test
{
	/**
	 * @brief The type of runner can be selected by the user when invoking tests, for example, via the command line
	 * interface. Additionally, the user can specify a specific runner for a specific test suite. Test suites must be
	 * registered with runtime registration to avoid having every translation unit recompile. With
	 * these two constraints its clear that the Runner interface must be runtime polymorphic.
	 *	Handles the interface to the test std::execution context.
	 */
	export class Runner
	{
	public:
		/**
		 * @brief
		 */
		explicit consteval Runner(std::string_view name);

		virtual ~Runner() = default;

		/**
		 * @brief
		 * @param function A function to execute on a scheduler
		 */
		virtual void Run(std::function_ref<void()> function) = 0;

		constexpr std::string_view Name() const;

	private:
		std::string_view name_;
	};

	consteval Runner::Runner(std::string_view name) :
		name_(name)
	{
	}

	constexpr std::string_view Runner::Name() const
	{
		return name_;
	}

	/**
	 * @brief Registers a runner object with the test framework
	 * @param runner The runner object to register
	 * @return Whether the runner was successfully registered
	 */
	export bool RegisterRunner(Runner& runner);

}
