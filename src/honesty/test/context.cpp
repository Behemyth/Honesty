module synodic.honesty.test:context;

import std;
import :runner;

namespace synodic::honesty::test
{
	// Emulating std::execution state
	class Context
	{
	public:
		explicit Context(Runner* runner, std::span<Reporter*> reporters);

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

	Context::Context(Runner* runner, const std::span<Reporter*> reporters) :
		reporters_(reporters),
		runner_(runner)
	{
	}

	void Context::Signal(const event::SuiteBegin& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::SuiteEnd& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::SuiteSkip& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::SuiteRun& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::SuiteFail& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::SuitePass& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::SuiteSummary& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::TestBegin& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::TestEnd& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::TestSkip& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::TestRun& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::TestFail& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::TestPass& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::AssertionFail& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::AssertionPass& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::AssertionSkip& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Signal(const event::Summary& event) const
	{
		for (Reporter* reporter: reporters_)
		{
			reporter->signal(event);
		}
	}

	void Context::Run(const std::function_ref<void()> function) const
	{
		if (runner_)
		{
			runner_->Run(function);
		}
	}
}

namespace
{
	// The fallback context for tests
	inline synodic::honesty::test::Context EMPTY_CONTEXT(nullptr, {});

	// Each thread has its own context, such that tests can reference global functions without an object
	inline thread_local synodic::honesty::test::Context& CONTEXT = EMPTY_CONTEXT;
}

synodic::honesty::test::Context& GetContext()
{
	return CONTEXT;
}
