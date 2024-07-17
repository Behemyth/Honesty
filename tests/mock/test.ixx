export module synodic.honesty.test.mock;

import std;

import function_ref;

import synodic.honesty.test;

export class MockReporter final : public synodic::honesty::test::StreamingAdapter
{
public:
	explicit MockReporter(const synodic::honesty::log::Logger& logger) :
		StreamingAdapter(logger)
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
	explicit MockRunner(const synodic::honesty::log::Logger& logger) :
		Runner(logger)
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

	synodic::honesty::test::Generator
		Run(const synodic::honesty::test::Requirements& requirements,
			const std::function_ref<synodic::honesty::test::Generator(const synodic::honesty::test::Requirements&)>
				function) override
	{
		return function(requirements);
	}
};

namespace
{
	synodic::honesty::test::RunnerRegistrar<MockRunner> RUNNER_REGISTRAR;
	synodic::honesty::test::ReporterRegistrar<MockReporter> REPORTER_REGISTRAR;
}
