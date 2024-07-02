module synodic.honesty.test:context;

import synodic.honesty.log;
import std;
import synodic.honesty.utility;
import :runner;
import :reporter;
import :test;

namespace synodic::honesty::test
{
	class Context
	{
	public:
		constexpr Context(Runner* runner, const std::span<std::unique_ptr<Reporter>> reporters) :
			reporters_(reporters),
			runner_(runner)
		{
		}

		void Signal(const event::SuiteBegin& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::SuiteEnd& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::SuiteSkip& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::SuiteRun& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::SuiteFail& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::SuitePass& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::SuiteSummary& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::TestBegin& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::TestEnd& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::TestSkip& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::TestRun& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::TestFail& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::TestPass& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::AssertionFail& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::AssertionPass& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::AssertionSkip& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		void Signal(const event::Summary& event) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(event);
			}
		}

		inline void
			Run(const Requirements& requirements, const std::function_ref<void(const Requirements&)> function) const
		{
			runner_->Run(requirements, function);
		}

		inline void
			Run(
				const Requirements& requirements,
				const std::function_ref<Generator(const Requirements&)> function) const
		{
			runner_->Run(requirements, function);
		}

		[[nodiscard]] auto Reporters() const -> std::span<std::unique_ptr<Reporter>>
		{
			return reporters_;
		}

	private:
		std::span<std::unique_ptr<Reporter>> reporters_;
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

		void
			Run(
				const Requirements& requirements,
				const std::function_ref<Generator(const Requirements&)> function) override
		{
		}
	};
}
