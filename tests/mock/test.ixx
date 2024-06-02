export module synodic.honesty.test.mock;

import synodic.honesty.test;
import std;
import function_ref;

export class MockReporter final : public synodic::honesty::test::Reporter
{
public:
	explicit MockReporter(synodic::honesty::log::Logger logger) :
		Reporter(std::move(logger))
	{
	}

	static consteval std::string_view Name()
	{
		return "mock";
	}

	void Signal(const synodic::honesty::test::event::SuiteBegin& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::SuiteEnd& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::SuiteSkip& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::SuiteRun& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::SuiteFail& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::SuitePass& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::SuiteSummary& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::TestBegin& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::TestEnd& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::TestSkip& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::TestRun& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::TestFail& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::TestPass& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::AssertionFail& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::AssertionPass& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::AssertionSkip& event) override
	{
	}

	void Signal(const synodic::honesty::test::event::Summary& event) override
	{
	}
};

export class MockRunner final : public synodic::honesty::test::Runner
{
public:
	explicit MockRunner(synodic::honesty::log::Logger logger) :
		Runner(std::move(logger))
	{
	}

	static consteval std::string_view Name()
	{
		return "mock";
	}

	void
		Run(const synodic::honesty::test::Requirements& requirements,
			const std::function_ref<void(const synodic::honesty::test::Requirements&)> function) override
	{
	}
};

namespace
{
	synodic::honesty::test::RunnerRegistrar<MockRunner> RUNNER_REGISTRAR;
	synodic::honesty::test::ReporterRegistrar<MockReporter> REPORTER_REGISTRAR;
}
