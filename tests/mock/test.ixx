export module synodic.honesty.test.mock;

import synodic.honesty.test;
import std;

export class MockReporter final : public synodic::honesty::test::Reporter
{
public:
	consteval MockReporter();

	void signal(const synodic::honesty::test::event::SuiteBegin& event) override;
	void signal(const synodic::honesty::test::event::SuiteEnd& event) override;
	void signal(const synodic::honesty::test::event::SuiteSkip& event) override;
	void signal(const synodic::honesty::test::event::SuiteRun& event) override;
	void signal(const synodic::honesty::test::event::SuiteFail& event) override;
	void signal(const synodic::honesty::test::event::SuitePass& event) override;
	void signal(const synodic::honesty::test::event::SuiteSummary& event) override;
	void signal(const synodic::honesty::test::event::TestBegin& event) override;
	void signal(const synodic::honesty::test::event::TestEnd& event) override;
	void signal(const synodic::honesty::test::event::TestSkip& event) override;
	void signal(const synodic::honesty::test::event::TestRun& event) override;
	void signal(const synodic::honesty::test::event::TestFail& event) override;
	void signal(const synodic::honesty::test::event::TestPass& event) override;
	void signal(const synodic::honesty::test::event::AssertionFail& event) override;
	void signal(const synodic::honesty::test::event::AssertionPass& event) override;
	void signal(const synodic::honesty::test::event::AssertionSkip& event) override;
	void signal(const synodic::honesty::test::event::Summary& event) override;
};

consteval MockReporter::MockReporter() :
	Reporter("mock")
{
}

export class MockRunner final : public synodic::honesty::test::Runner
{
public:
	consteval MockRunner();

	void Run(synodic::honesty::test::Broadcast& broadcaster) override;
	void Submit(const synodic::honesty::test::SuiteData* data) override;
	void Submit(std::span<const synodic::honesty::test::SuiteData* const> data) override;
};

consteval MockRunner::MockRunner() :
	Runner("mock")
{
}

void MockReporter::signal(const synodic::honesty::test::event::SuitePass& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::SuiteRun& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::TestRun& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::SuiteBegin& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::SuiteEnd& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::SuiteSkip& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::SuiteFail& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::SuiteSummary& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::TestBegin& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::TestEnd& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::TestSkip& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::TestFail& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::TestPass& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::AssertionFail& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::AssertionPass& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::AssertionSkip& event)
{
}

void MockReporter::signal(const synodic::honesty::test::event::Summary& event)
{
}

void MockRunner::Run(synodic::honesty::test::Broadcast& broadcaster)
{
}

void MockRunner::Submit(const synodic::honesty::test::SuiteData* data)
{
}

void MockRunner::Submit(std::span<const synodic::honesty::test::SuiteData* const> data)
{
}
