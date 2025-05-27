export module synodic.honesty.test.mock;

import std;

import function_ref;

import synodic.honesty.test;

using namespace honesty::test;

export class MockReporter final : public StreamingAdapter
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

export class MockRunner final : public Runner
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

	void Run(
		const Requirements& requirements,
		const std::function_ref<void(const Requirements&)> function) override
	{
		function(requirements);
	}

	Generator Run(
		const Requirements& requirements,
		const std::function_ref<Generator(const Requirements&)> function) override
	{
		return function(requirements);
	}

	Generator Run(const std::function_ref<Generator()> function) override
	{
		return function();
	}

	Generator Run(
		Fixture& fixture,
		const std::function_ref<Generator(Fixture&)> function) override
	{
		return function(fixture);
	}
};

namespace
{
	RunnerRegistrar<MockRunner> RUNNER_REGISTRAR;
	ReporterRegistrar<MockReporter> REPORTER_REGISTRAR;
}
