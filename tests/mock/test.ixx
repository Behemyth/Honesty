export module synodic.honesty.test.mock;

import std;

import function_ref;

import synodic.honesty.test;

export class MockReporter final : public honesty::test::StreamingAdapter
{
public:
	explicit MockReporter(const honesty::log::Logger& logger) :
		StreamingAdapter(logger)
	{
	}

	static consteval std::string_view Name()
	{
		return "mock";
	}
};

export class MockRunner final : public honesty::test::Runner
{
public:
	explicit MockRunner(const honesty::log::Logger& logger) :
		Runner(logger)
	{
	}

	static consteval std::string_view Name()
	{
		return "mock";
	}

	void
		Run(const honesty::test::Requirements& requirements,
			const std::function_ref<void(const honesty::test::Requirements&)> function) override
	{
		function(requirements);
	}

	honesty::test::Generator
		Run(const std::function_ref<honesty::test::Generator()> function) override
	{
		return function();
	}

	honesty::test::Generator Run(
		honesty::test::Fixture& fixture,
		const std::function_ref<honesty::test::Generator(honesty::test::Fixture&)> function) override
	{
		return function(fixture);
	}
};

namespace
{
	honesty::test::RunnerRegistrar<MockRunner> RUNNER_REGISTRAR;
	honesty::test::ReporterRegistrar<MockReporter> REPORTER_REGISTRAR;
}
