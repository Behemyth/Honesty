
export module synodic.honesty.test:runner;

import :types;
import :reporter;
import std;

namespace synodic::honesty::test
{

	export class Broadcast final : public Reporter
	{
	public:
		Broadcast()			  = default;
		~Broadcast() override = default;

		void signal(const event::SuiteBegin& event) override;
		void signal(const event::SuiteEnd& event) override;
		void signal(const event::SuiteSkip& event) override;

		void signal(const event::SuiteRun& event) override;
		void signal(const event::SuiteFail& event) override;
		void signal(const event::SuitePass& event) override;

		void signal(const event::SuiteSummary& event) override;

		void signal(const event::TestBegin& event) override;
		void signal(const event::TestEnd& event) override;
		void signal(const event::TestSkip& event) override;

		void signal(const event::TestRun& event) override;
		void signal(const event::TestFail& event) override;
		void signal(const event::TestPass& event) override;

		void signal(const event::AssertionFail& event) override;
		void signal(const event::AssertionPass& event) override;
		void signal(const event::AssertionSkip& event) override;

		void signal(const event::Summary& event) override;
	};

	/**
	 * \brief The type of runner can be selected by the user when invoking tests, for example, via the command line
	 * interface. Additionally, the user can specify a specific runner for a specific test suite. Test suites must be
	 * registered with runtime registration to avoid having every translation unit recompile when mo. With
	 * these two constraints its clear that the Runner interface must be runtime polymorphic.
	 *	Handles the interface to the test std::execution context.
	 */
	export class Runner
	{
	public:
		/**
		 * @brief
		 */
		explicit constexpr Runner();

		virtual ~Runner() = default;

		virtual void Run(Broadcast& broadcaster) = 0;

		virtual void Submit(const SuiteData* data)					= 0;
		virtual void Submit(std::span<const SuiteData* const> data) = 0;
	};

	constexpr Runner::Runner()
	{
	}

	// Emulating std::execution state
	export class RunnerContext
	{
	public:
		RunnerContext() = default;

		Broadcast broadcaster;

	private:
	};
}
