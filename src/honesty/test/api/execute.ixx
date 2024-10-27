export module synodic.honesty.test.api:execute;

import std;

import synodic.honesty.log;
import synodic.honesty.test.backend;

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
			const std::string_view header,
			const log::Logger& logger) :
			applicationName(applicationName),
			filter(filter),
			runner(runner),
			reporters(reporters),
			dryRun(dryRun),
			header(header),
			logger(logger)
		{
		}

		std::string_view applicationName;
		std::string_view filter;

		std::reference_wrapper<Runner> runner;
		std::span<std::unique_ptr<Reporter>> reporters;

		bool dryRun;
		std::string_view header;
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

	bool ProcessTest(Runner& runner, const TestData& testData, const TestContext& testContext)
	{
		bool success = true;

		// Filter the test by name
		if (not testContext.filterViews.empty() and testData.Name() != testContext.filterViews.front())
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

		for (const std::unique_ptr<Reporter>& reporter: testContext.reporters)
		{
			reporter->Signal(testBegin);
		}

		const Requirements requirements = testContext.CreateRequirements(testData.Name(), testOutcome);

		auto testExecutor = Overload {
			[&](const std::function_ref<void(const Requirements&)>& testCallback)
			{
				if (testData.Tag() == "skip")
				{
					event::TestSkip testSkip;
					testSkip.name = testData.Name();

					for (const std::unique_ptr<Reporter>& reporter: testContext.reporters)
					{
						reporter->Signal(testSkip);
					}

					return;
				}

				if (not testContext.dryRun)
				{
					runner.Run(requirements, testCallback);
				}
			},
			[&](const std::function_ref<Generator()>& testCallback)
			{
				Generator generator = runner.Run(testCallback);

				std::span filter = testContext.filterViews;

				// Move to the next filter part
				filter = filter | std::ranges::views::drop(1);

				for (const Test& test: generator)
				{
					TestContext newContext(
						testContext.reporters,
						testContext.logger.CreateLogger(test.Name()),
						filter,
						testContext.dryRun);

					if (not ProcessTest(runner, static_cast<TestData>(test), newContext))
					{
						success = false;
						break;
					}
				}
			}};

		// Start the recursive test execution
		std::visit(testExecutor, testData.Variant());

		// Get the output from the test
		TestContext::OutputData testOutput = testContext.Output(requirements);

		if (not testOutput.success)
		{
			success = false;
		}

		event::TestEnd testEnd;
		testEnd.name = testData.Name();

		for (const std::unique_ptr<Reporter>& reporter: testContext.reporters)
		{
			reporter->Signal(testEnd);
		}

		return success;
	}

	bool ProcessSuite(Runner& runner, const SuiteData& suite, SuiteContext& suiteContext)
	{
		bool success = true;

		std::span filter = suiteContext.filterViews;

		// Before we start, check to see if we have a filter
		if (not suiteContext.filterViews.empty())
		{
			// Check if the suite name matches the filter
			if (suite.Name() != suiteContext.filterViews.front())
			{
				return true;
			}

			// Move to the next filter part
			filter = filter | std::ranges::views::drop(1);
		}

		event::SuiteBegin suiteBegin;
		suiteBegin.name = suite.Name();

		for (std::unique_ptr<Reporter>& reporter: suiteContext.reporters)
		{
			reporter->Signal(suiteBegin);
		}

		// Fixture lifetime should be for the whole suite
		Fixture fixture = suiteContext.CreateFixture();

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
			const auto& view = static_cast<TestData>(test);

			bool testSuccess = true;

			TestContext testContext(
				suiteContext.reporters,
				suiteContext.logger.CreateLogger(view.Name()),
				filter,
				suiteContext.dryRun);

			testSuccess = ProcessTest(runner, view, testContext);

			if (not testSuccess)
			{
				success = false;
			}
		}

		event::SuiteEnd end;
		end.name = suite.Name();

		for (const std::unique_ptr<Reporter>& reporter: suiteContext.reporters)
		{
			reporter->Signal(end);
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

		std::span filterViews = filterData;

		bool success = true;

		const log::Logger& logger = parameters.logger.get();

		const std::string threadName = std::format("{}", std::this_thread::get_id());
		const std::uint64_t seed	 = std::random_device()();

		const event::Initialize initialize(seed, parameters.header);
		for (const std::unique_ptr<Reporter>& reporter: parameters.reporters)
		{
			reporter->Signal(initialize);
		}

		for (const SuiteData& suite: GetSuites())
		{
			SuiteContext suiteContext(
				parameters.reporters,
				logger.CreateLogger(threadName),
				parameters.applicationName,
				suite.Name(),
				filterViews,
				parameters.dryRun);

			if (not ProcessSuite(parameters.runner, suite, suiteContext))
			{
				success = false;
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
