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
		explicit Context(Runner& runner, std::span<Reporter*> reporters);

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

		inline void Run(std::function_ref<void()> function) const;

	private:
		std::span<Reporter*> reporters_;
		Runner* runner_;
	};

	Context::Context(Runner& runner, const std::span<Reporter*> reporters) :
		reporters_(reporters),
		runner_(&runner)
	{
	}

	void Context::Signal(const event::SuiteBegin& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::SuiteEnd& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::SuiteSkip& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::SuiteRun& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::SuiteFail& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::SuitePass& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::SuiteSummary& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::TestBegin& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::TestEnd& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::TestSkip& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::TestRun& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::TestFail& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::TestPass& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::AssertionFail& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::AssertionPass& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::AssertionSkip& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Signal(const event::Summary& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->Signal(event);
		}
	}

	void Context::Run(const std::function_ref<void()> function) const
	{
		runner_->Run(function);
	}
}

namespace
{
	constinit synodic::honesty::test::DefaultRunner DEFAULT_RUNNER("default");

	constinit std::array<synodic::honesty::test::Runner*, 1> RUNNERS = {&DEFAULT_RUNNER};

	constinit synodic::honesty::test::CompactReporter COMPACT_REPORTER("compact");
	constinit synodic::honesty::test::DefaultReporter DEFAULT_REPORTER("default");

	constinit std::array<synodic::honesty::test::Reporter*, 2> REPORTERS = {&COMPACT_REPORTER, &DEFAULT_REPORTER};

	// The default context for tests
	inline synodic::honesty::test::Context DEFAULT_CONTEXT(DEFAULT_RUNNER, {});

	// Each thread has its own context, such that tests can reference global functions without an object
	inline thread_local synodic::honesty::test::Context& CONTEXT = DEFAULT_CONTEXT;
}

synodic::honesty::test::Context& GetContext()
{
	return CONTEXT;
}

std::span<synodic::honesty::test::Reporter*> GetBuiltinReporters()
{
	return REPORTERS;
}

const synodic::honesty::test::Reporter& GetDefaultReporter()
{
	return DEFAULT_REPORTER;
}

std::span<synodic::honesty::test::Runner*> GetBuiltinRunners()
{
	return RUNNERS;
}

const synodic::honesty::test::Runner& GetDefaultRunner()
{
	return DEFAULT_RUNNER;
}
