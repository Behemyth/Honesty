
export module synodic.honesty.test.backend:runner;

import :test;
import :suite;
import std;

export namespace synodic::honesty
{
	/**
	 * \brief The type of runner can be selected by the user when invoking tests, for example, via the command line
	 * interface. Additionally, the user can specify a specific runner for a specific test suite. Test suites must be
	 * registered with static runtime registration to overcome the limitations of the C++ compile time process. With
	 * these two constraints its clear that the Runner interface must be runtime polymorphic.
	 */
	class Runner
	{
	public:
		/**
		 * \brief
		 * \param name The name of the runner. This is used to identify the runner when invoking tests
		 */
		constexpr Runner(std::string_view name);

		virtual ~Runner() = default;

		virtual void Run() = 0;

		virtual void Submit(suite_data data)			  = 0;
		virtual void Submit(std::vector<suite_data> data) = 0;

	protected:
		const std::string_view name_;
	};

	constexpr Runner::Runner(std::string_view name) :
		name_(name)
	{
	}
}
