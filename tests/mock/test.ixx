export module synodic.honesty.test.mock;

import synodic.honesty.test;
import std;
import function_ref;

export class MockReporter final : public synodic::honesty::test::StreamingAdapter
{
public:
	explicit MockReporter(synodic::honesty::log::Logger logger) :
		StreamingAdapter(std::move(logger))
	{
	}

	static consteval std::string_view Name()
	{
		return "mock";
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
		function(requirements);
	}
};

namespace
{
	synodic::honesty::test::RunnerRegistrar<MockRunner> RUNNER_REGISTRAR;
	synodic::honesty::test::ReporterRegistrar<MockReporter> REPORTER_REGISTRAR;
}
