export module synodic.honesty.test.api:execute;

import std;

import synodic.honesty.log;
import synodic.honesty.test.backend;
import synodic.honesty.test.types;

import function_ref;

namespace
{
	// A helper for variant overload deduction
	template<typename... Ts>
	struct Overload : Ts...
	{
		using Ts::operator()...;
	};

}

namespace synodic::honesty::test::api
{

	export struct ExecuteParameters
	{
		ExecuteParameters(
			const std::string_view applicationName,
			const std::string_view filter,
			Runner& runner,
			const std::span<std::unique_ptr<Reporter>> reporters,
			const bool dryRun,
			const log::Logger& logger) :
			applicationName(applicationName),
			filter(filter),
			runner(runner),
			reporters(reporters),
			dryRun(dryRun),
			logger(logger)
		{
		}

		std::string_view applicationName;
		std::string_view filter;

		std::reference_wrapper<Runner> runner;
		std::span<std::unique_ptr<Reporter>> reporters;

		bool dryRun;
		std::reference_wrapper<const log::Logger> logger;
	};

	export struct ExecuteResult
	{
		explicit ExecuteResult(const bool success) :
			success(success)
		{
		}

		bool success;
	};

	bool ProcessTest(const TestData& testData, std::span<std::string_view> filter, const ExecuteParameters& parameters)
	{
		bool success = true;

		// Filter the test by name
		if (not filter.empty() and testData.Name() != filter.front())
		{
			return success;
		}

		auto testOutcome   = ExpectedTestOutcome::PASS;
		auto assertOutcome = ExpectedAssertOutcome::PASS;

		// Write the outcomes based on the input tags. The tags should already be constrained.
		{
			if (testData.Tag() == "fail")
			{
				testOutcome	  = ExpectedTestOutcome::FAIL;
				assertOutcome = ExpectedAssertOutcome::FAIL;
			}
			else if (testData.Tag() == "skip")
			{
				testOutcome = ExpectedTestOutcome::SKIP;
			}
			else if (testData.Tag() == "todo")
			{
				testOutcome	  = ExpectedTestOutcome::TODO;
				assertOutcome = ExpectedAssertOutcome::TODO;
			}
		}

		const event::TestBegin testBegin(testData.Name(), assertOutcome);

		for (const std::unique_ptr<Reporter>& reporter: parameters.reporters)
		{
			reporter->Signal(testBegin);
		}

		const RequirementParameters requirementParameters(testData.Name(), testOutcome);
		const RequirementContext requirements(parameters.reporters, requirementParameters, parameters.logger);

		Runner& runner = parameters.runner.get();

		auto testExecutor = Overload {
			[&](const std::function_ref<void(const Requirements&)>& testCallback)
			{
				runner.Run(requirements, testCallback);
			},
			[&](const std::function_ref<Generator(const Requirements&)>& testCallback)
			{
				Generator generator = runner.Run(requirements, testCallback);

				for (const Test& test: generator)
				{
					if (not ProcessTest(static_cast<TestData>(test), filter, parameters))
					{
						success = false;
						break;
					}
				}
			}};

		// Start the recursive test execution
		std::visit(testExecutor, testData.Variant());

		if (not requirements.Output().success)
		{
			success = false;
		}

		event::TestEnd testEnd;
		testEnd.name = testData.Name();

		for (std::unique_ptr<Reporter>& reporter: parameters.reporters)
		{
			reporter->Signal(testEnd);
		}

		return success;
	}

	export auto Execute(const ExecuteParameters& parameters) -> ExecuteResult
	{
		// Break down the filter into individual views
		auto splitData = parameters.filter | std::ranges::views::split('.') |
						 std::ranges::views::transform(
							 [](auto&& str)
							 {
								 return std::string_view(str.data(), std::ranges::distance(str));
							 });

		std::vector<std::string_view> filterData = std::ranges::to<std::vector>(splitData);

		std::span filter = filterData;

		bool success = true;

		for (const SuiteData& suite: GetSuites())
		{
			// Before we start, check to see if we have a filter
			if (not parameters.filter.empty())
			{
				// Check if the suite name matches the filter
				if (suite.Name() != filter.front())
				{
					continue;
				}

				// Move to the next filter part
				filter = filter | std::ranges::views::drop(1);
			}

			event::SuiteBegin suiteBegin;
			suiteBegin.name = suite.Name();

			for (std::unique_ptr<Reporter>& reporter: parameters.reporters)
			{
				reporter->Signal(suiteBegin);
			}

			// Fixture lifetime should be for the whole suite
			Fixture fixture(parameters.applicationName, suite.Name(), parameters.logger);

			auto executor = Overload {
				[&](const std::function_ref<Generator()> generator) -> Generator
				{
					return generator();
				},
				[&](const std::function_ref<Generator(Fixture&)>& generator) -> Generator
				{
					return generator(fixture);
				}};

			Generator generator = std::visit(executor, suite.Variant());

			for (const Test& test: generator)
			{
				const TestData& view = static_cast<TestData>(test);

				bool testSuccess = true;

				if (not parameters.dryRun)
				{
					testSuccess = ProcessTest(view, filter, parameters);
				}

				if (not testSuccess)
				{
					success = false;
				}
			}

			event::SuiteEnd end;
			end.name = suite.Name();

			for (const std::unique_ptr<Reporter>& reporter: parameters.reporters)
			{
				reporter->Signal(end);
			}
		}

		const event::Summary summary;
		for (const std::unique_ptr<Reporter>& reporter: parameters.reporters)
		{
			reporter->Signal(summary);
		}

		return ExecuteResult(success);
	}
}
