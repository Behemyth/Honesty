
export module synodic.honesty.test:runner;

import :types;
import :reporter;
import std;

namespace synodic::honesty::test
{

	export class Broadcast final
	{
	public:
		explicit Broadcast(std::span<Reporter*> reporters);

		~Broadcast() = default;

		void Signal(const event::SuiteBegin& event) const;
		void Signal(const event::SuiteEnd& event) const;
		void Signal(const event::SuiteSkip& event) const;

		void Signal(const event::SuiteRun& event) const;
		void Signal(const event::SuiteFail& event) const;
		void Signal(const event::SuitePass& event) const;

		void Signal(const event::SuiteSummary& event) const;

		void Signal(const event::TestBegin& event) const;
		void Signal(const event::TestEnd& event) const;
		void Signal(const event::TestSkip& event) const;

		void Signal(const event::TestRun& event) const;
		void Signal(const event::TestFail& event) const;
		void Signal(const event::TestPass& event) const;

		void Signal(const event::AssertionFail& event) const;
		void Signal(const event::AssertionPass& event) const;
		void Signal(const event::AssertionSkip& event) const;

		void Signal(const event::Summary& event) const;

	private:
		std::span<Reporter*> reporters_;
	};

	// Emulating std::execution state
	export class RunnerContext
	{
	public:
		explicit RunnerContext(std::span<Reporter*> reporters);

		Broadcast broadcaster;

	private:
	};

	/**
	 * @brief A wrapper around a test suite that provides automatic management of test data fit for runner usage
	 */
	export class Set
	{
	public:
		Set(const SuiteData& data);

		std::string_view Name() const;


	private:
		std::string_view name_;
	};

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
		 * @param generateContext A function that generates a newly created thread local context
		 */
		virtual void Run(std::function_ref<void(RunnerContext& context)> generateContext) = 0;

		constexpr std::string_view Name() const;

	private:
		std::string_view name_;
	};

	/**
	 * @brief Registers a runner object with the test framework
	 * @param runner The runner object to register
	 * @return Whether the runner was successfully registered
	 */
	export bool RegisterRunner(Runner& runner);

	consteval Runner::Runner(std::string_view name) :
		name_(name)
	{
	}

	constexpr std::string_view Runner::Name() const
	{
		return name_;
	}
}
