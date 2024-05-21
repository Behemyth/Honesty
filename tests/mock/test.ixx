export module synodic.honesty.test.mock;

import synodic.honesty.test;
import std;
import function_ref;

export class MockReporter final : public synodic::honesty::test::Reporter
{
public:
	MockReporter();

	void Signal(const synodic::honesty::test::event::SuiteBegin& event) override;
	void Signal(const synodic::honesty::test::event::SuiteEnd& event) override;
	void Signal(const synodic::honesty::test::event::SuiteSkip& event) override;
	void Signal(const synodic::honesty::test::event::SuiteRun& event) override;
	void Signal(const synodic::honesty::test::event::SuiteFail& event) override;
	void Signal(const synodic::honesty::test::event::SuitePass& event) override;
	void Signal(const synodic::honesty::test::event::SuiteSummary& event) override;
	void Signal(const synodic::honesty::test::event::TestBegin& event) override;
	void Signal(const synodic::honesty::test::event::TestEnd& event) override;
	void Signal(const synodic::honesty::test::event::TestSkip& event) override;
	void Signal(const synodic::honesty::test::event::TestRun& event) override;
	void Signal(const synodic::honesty::test::event::TestFail& event) override;
	void Signal(const synodic::honesty::test::event::TestPass& event) override;
	void Signal(const synodic::honesty::test::event::AssertionFail& event) override;
	void Signal(const synodic::honesty::test::event::AssertionPass& event) override;
	void Signal(const synodic::honesty::test::event::AssertionSkip& event) override;
	void Signal(const synodic::honesty::test::event::Summary& event) override;
};

MockReporter::MockReporter()
{
}

export class MockRunner final : public synodic::honesty::test::Runner
{
public:
	consteval MockRunner();

	void Run(std::function_ref<void()> function) override;
};

consteval MockRunner::MockRunner() :
	Runner("mock")
{
}

void MockReporter::Signal(const synodic::honesty::test::event::SuitePass& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::SuiteRun& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::TestRun& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::SuiteBegin& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::SuiteEnd& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::SuiteSkip& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::SuiteFail& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::SuiteSummary& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::TestBegin& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::TestEnd& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::TestSkip& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::TestFail& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::TestPass& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::AssertionFail& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::AssertionPass& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::AssertionSkip& event)
{
}

void MockReporter::Signal(const synodic::honesty::test::event::Summary& event)
{
}

void MockRunner::Run(std::function_ref<void()> function)
{
}
