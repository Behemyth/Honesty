module synodic.honesty.test:context;

import std;
import synodic.honesty.utility;
import :runner;
import :runner.default;
import :reporter;
import :reporter.compact;
import :reporter.default;

namespace synodic::honesty::test
{
	// Emulating std::execution state
	class Context
	{
	public:
		constexpr Context(Runner& runner, const std::span<Reporter*> reporters) :
			reporters_(reporters),
			runner_(&runner)
		{
		}

		void Signal(const event::SuiteBegin& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::SuiteEnd& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::SuiteSkip& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::SuiteRun& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::SuiteFail& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::SuitePass& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::SuiteSummary& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::TestBegin& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::TestEnd& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::TestSkip& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::TestRun& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::TestFail& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::TestPass& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::AssertionFail& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::AssertionPass& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::AssertionSkip& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::Summary& event) const
		{
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		inline void Run(const std::function_ref<void()> function) const
		{
			runner_->Run(function);
		}

	private:
		std::span<Reporter*> reporters_;
		Runner* runner_;
	};

	/**
	 * @brief An empty runner that throws an exception if it is used as a runner. We want instances where the default
	 * runner isn't replaced to notify the developer
	 */
	class EmptyRunner final : public Runner
	{
	public:
		consteval EmptyRunner() :
			Runner("empty")
		{
		}

		void Run(std::function_ref<void()> function) override
		{
			throw utility::NotImplemented();
		}
	};

	/**
	 * @brief An empty reporter that does nothing
	 */
	class EmptyReporter final : public StreamingAdapter
	{
	public:
		consteval EmptyReporter() = default;

		static consteval auto Name() -> std::string_view
		{
			return "empty";
		}
	};
}

namespace
{
	constinit synodic::honesty::test::EmptyRunner EMPTY_RUNNER;
	constinit synodic::honesty::test::EmptyReporter EMPTY_REPORTER;

	// The default reporters for tests.
	constinit std::array<synodic::honesty::test::Reporter*, 1> DEFAULT_REPORTERS = {
		&EMPTY_REPORTER,
	};

	// The default context for tests
	constinit synodic::honesty::test::Context EMPTY_CONTEXT(EMPTY_RUNNER, DEFAULT_REPORTERS);

	// Each thread has its own context, such that tests can reference global functions without an object
	constinit thread_local synodic::honesty::test::Context& CONTEXT = EMPTY_CONTEXT;
}

synodic::honesty::test::Context& GetContext()
{
	return CONTEXT;
}
