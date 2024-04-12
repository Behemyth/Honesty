import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	class MockReporter final : public Reporter
	{
	public:
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

	class MockRunner final : public Runner
	{
	public:
		void Run(Broadcast& broadcaster) override;
		void Submit(const SuiteData* data) override;
		void Submit(std::span<const SuiteData* const> data) override;
	};

	auto interfaceGenerator = []() -> generator<TestBase>
	{
		co_yield "list"_test = []()
		{
			MockReporter reporter;
			MockRunner runner;

			Interface::Configuration configuration;

			Interface interface(configuration);

			ListParameters parameters;
			auto result = interface.List(parameters);

			expect_equals(0, 0);
		};
	};

	Suite suite("suite", interfaceGenerator);
	bool result = suite.Register();

	void MockReporter::signal(const event::SuitePass& event)
	{
	}

	void MockReporter::signal(const event::SuiteRun& event)
	{
	}

	void MockReporter::signal(const event::TestRun& event)
	{
	}

	void MockReporter::signal(const event::SuiteBegin& event)
	{
	}

	void MockReporter::signal(const event::SuiteEnd& event)
	{
	}

	void MockReporter::signal(const event::SuiteSkip& event)
	{
	}

	void MockReporter::signal(const event::SuiteFail& event)
	{
	}

	void MockReporter::signal(const event::SuiteSummary& event)
	{
	}

	void MockReporter::signal(const event::TestBegin& event)
	{
	}

	void MockReporter::signal(const event::TestEnd& event)
	{
	}

	void MockReporter::signal(const event::TestSkip& event)
	{
	}

	void MockReporter::signal(const event::TestFail& event)
	{
	}

	void MockReporter::signal(const event::TestPass& event)
	{
	}

	void MockReporter::signal(const event::AssertionFail& event)
	{
	}

	void MockReporter::signal(const event::AssertionPass& event)
	{
	}

	void MockReporter::signal(const event::AssertionSkip& event)
	{
	}

	void MockReporter::signal(const event::Summary& event)
	{
	}

	void MockRunner::Run(Broadcast& broadcaster)
	{
	}

	void MockRunner::Submit(const SuiteData* data)
	{
	}

	void MockRunner::Submit(std::span<const SuiteData* const> data)
	{
	}
}
