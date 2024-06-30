module synodic.honesty.test:context;

import synodic.honesty.log;
import std;
import synodic.honesty.utility;
import :runner;
import :reporter;

namespace synodic::honesty::test
{
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

		inline void
			Run(const Requirements& requirements, const std::function_ref<void(const Requirements&)> function) const
		{
			runner_->Run(requirements, function);
		}

		[[nodiscard]] auto Reporters() const -> std::span<Reporter*>
		{
			return reporters_;
		}

	private:
		std::span<Reporter*> reporters_;
		Runner* runner_;
	};

	class EmptyRunner final : public Runner
	{
	public:
		explicit(false) EmptyRunner(const log::Logger& logger) :
			Runner(logger)
		{
		}

		void Run(const Requirements& requirements, const std::function_ref<void(const Requirements&)> function) override
		{
		}
	};
}
