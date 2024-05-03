module synodic.honesty.test:context;

import std;
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
		constexpr Context(Runner& runner, std::span<Reporter*> reporters) :
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

		inline void Run(std::function_ref<void()> function) const
		{
			runner_->Run(function);
		}

	private:
		std::span<Reporter*> reporters_;
		Runner* runner_;
	};
}

// TODO: Move data to single object
namespace
{
	constinit synodic::honesty::test::DefaultRunner DEFAULT_RUNNER("default");

	constinit std::array<synodic::honesty::test::Runner*, 1> RUNNERS = {&DEFAULT_RUNNER};

	constinit synodic::honesty::test::CompactReporter COMPACT_REPORTER("compact");
	constinit synodic::honesty::test::DefaultReporter DEFAULT_REPORTER("default");

	constinit std::array<synodic::honesty::test::Reporter*, 2> PUBLIC_REPORTERS = {
		&COMPACT_REPORTER,
		&DEFAULT_REPORTER};

	// The default reporters for tests.
	constinit std::array<synodic::honesty::test::Reporter*, 1> DEFAULT_REPORTERS = {
		&DEFAULT_REPORTER,
	};

	// The default context for tests
	constinit synodic::honesty::test::Context DEFAULT_CONTEXT(DEFAULT_RUNNER, DEFAULT_REPORTERS);

	// Each thread has its own context, such that tests can reference global functions without an object
	constinit thread_local synodic::honesty::test::Context& CONTEXT = DEFAULT_CONTEXT;
}

synodic::honesty::test::Context& GetContext()
{
	return CONTEXT;
}

std::span<synodic::honesty::test::Reporter*> GetPublicReporters()
{
	return PUBLIC_REPORTERS;
}

synodic::honesty::test::Reporter& GetDefaultReporter()
{
	return DEFAULT_REPORTER;
}

synodic::honesty::test::Reporter& GetCompactReporter()
{
	return COMPACT_REPORTER;
}

std::span<synodic::honesty::test::Runner*> GetPublicRunners()
{
	return RUNNERS;
}

synodic::honesty::test::Runner& GetDefaultRunner()
{
	return DEFAULT_RUNNER;
}
