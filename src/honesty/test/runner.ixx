
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
		explicit constexpr Runner(const std::string_view name) :
			name_(name)
		{
		}

		virtual ~Runner() = default;

		/**
		 * @brief
		 * @param function A function to execute on a scheduler
		 */
		virtual void Run(std::function_ref<void()> function) = 0;

		constexpr std::string_view Name() const
		{
			return name_;
		}

	private:
		std::string_view name_;
	};
}
