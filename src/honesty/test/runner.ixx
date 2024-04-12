
export module synodic.honesty.test:runner;

import :types;
import :reporter;
import std;

namespace synodic::honesty::test
{

	export class Broadcast final
	{
	public:
		Broadcast(std::span<Reporter*> reporters);

		~Broadcast() = default;

		void signal(const event::SuiteBegin& event) const;
		void signal(const event::SuiteEnd& event) const;
		void signal(const event::SuiteSkip& event) const;

		void signal(const event::SuiteRun& event) const;
		void signal(const event::SuiteFail& event) const;
		void signal(const event::SuitePass& event) const;

		void signal(const event::SuiteSummary& event) const;

		void signal(const event::TestBegin& event) const;
		void signal(const event::TestEnd& event) const;
		void signal(const event::TestSkip& event) const;

		void signal(const event::TestRun& event) const;
		void signal(const event::TestFail& event) const;
		void signal(const event::TestPass& event) const;

		void signal(const event::AssertionFail& event) const;
		void signal(const event::AssertionPass& event) const;
		void signal(const event::AssertionSkip& event) const;

		void signal(const event::Summary& event) const;

	private:
		std::span<Reporter*> reporters_;
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

		virtual void Run(Broadcast& broadcaster) = 0;

		virtual void Submit(const SuiteData* data)					= 0;
		virtual void Submit(std::span<const SuiteData* const> data) = 0;

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

	// Emulating std::execution state
	export class RunnerContext
	{
	public:
		RunnerContext(std::span<Reporter*> reporters);

		Broadcast broadcaster;

	private:
	};
}
