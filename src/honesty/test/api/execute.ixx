export module synodic.honesty.test.api:execute;

import std;

import synodic.honesty.log;
import synodic.honesty.test.backend;
import synodic.honesty.metric;

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

namespace honesty::test::api
{
	export struct ExecuteParameters
	{
		ExecuteParameters(
			const std::string_view applicationName,
			const std::string_view filter,
			const bool dryRun,
			const std::string_view header
			) :
			applicationName(applicationName),
			filter(filter),
			dryRun(dryRun),
			header(header)
		{
		}

		std::string_view applicationName;
		std::string_view filter;

		bool dryRun;
		std::string_view header;
	};

	export struct ExecuteResult
	{
		explicit ExecuteResult(const bool success) :
			success(success)
		{
		}

		bool success;
	};

	// Forward declaration for ProcessTest so it can be used in helpers below
	bool ProcessTest(Runner& runner, const TestData& testData, const TestContext& testContext);

	bool HandleTags(
		const TestData& testData,
		const TestContext& testContext)
	{
		const bool todo = testData.Tag() == "todo";
		if (testData.Tag() == "skip" || todo)
		{
			event::TestSkip testSkip;
			testSkip.name = testData.Name();
			testSkip.todo = todo;

			for (Reporter* reporter: testContext.reporters)
			{
				reporter->Signal(testSkip);
			}
			return true;
		}
		return false;
	}

	bool HandleCallback(
		Runner& runner,
		const TestData& testData,
		const TestContext& testContext,
		const Requirements& requirements,
		const std::function_ref<void(const Requirements&)>& testCallback)
	{
		if (HandleTags(testData, testContext))
		{
			return true;
		}

		if (not testContext.dryRun)
		{
			runner.Run(requirements, testCallback);
		}
		return true;
	}

	bool HandleCallback(
		Runner& runner,
		const TestData& testData,
		const TestContext& testContext,
		const Requirements& requirements,
		const std::function_ref<Generator(const Requirements&)>& testCallback)
	{
		if (HandleTags(testData, testContext))
		{
			return true;
		}

		if (not testContext.dryRun)
		{
			Generator generator = testCallback(requirements);

			for (const Test& test: generator)
			{
				TestContext newContext(
					testContext.reporters,
					testContext.logger,
					testContext.filterViews,
					testContext.dryRun);

				if (not ProcessTest(runner, static_cast<TestData>(test), newContext))
				{
					return false;
				}
			}
		}
		return true;
	}

	bool HandleCallback(
		Runner& runner,
		const TestData& testData,
		const TestContext& testContext,
		const std::function_ref<Generator()>& testCallback)
	{
		Generator generator = runner.Run(testCallback);

		std::span filter = testContext.filterViews;
		filter           = filter | std::ranges::views::drop(1);

		for (const Test& test: generator)
		{
			TestContext newContext(
				testContext.reporters,
				testContext.logger,
				filter,
				testContext.dryRun);

			if (not ProcessTest(runner, static_cast<TestData>(test), newContext))
			{
				return false;
			}
		}
		return true;
	}

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
				testOutcome   = ExpectedTestOutcome::FAIL;
				assertOutcome = ExpectedAssertOutcome::FAIL;
			}
			else if (testData.Tag() == "skip")
			{
				testOutcome = ExpectedTestOutcome::SKIP;
			}
			else if (testData.Tag() == "todo")
			{
				testOutcome   = ExpectedTestOutcome::TODO;
				assertOutcome = ExpectedAssertOutcome::TODO;
			}
		}

		const event::TestBegin testBegin(testData.Name(), assertOutcome);

		for (Reporter* reporter: testContext.reporters)
		{
			reporter->Signal(testBegin);
		}

		metric::Duration duration;

		{
			metric::Timer timer(duration);

			const Requirements requirements = testContext.CreateRequirements(testData.Name(), testOutcome);

			auto testExecutor = Overload{
				[&](const std::function_ref<void(const Requirements&)>& testCallback)
				{
					success = HandleCallback(runner, testData, testContext, requirements, testCallback);
				},
				[&](const std::function_ref<Generator(const Requirements&)>& testCallback)
				{
					success = HandleCallback(
						runner,
						testData,
						testContext,
						requirements,
						testCallback);
				},
				[&](const std::function_ref<Generator()>& testCallback)
				{
					success = HandleCallback(runner, testData, testContext, testCallback);
				},
				[&](std::monostate)
				{
					// No test callback provided; treat as skipped
					event::TestSkip testSkip;
					testSkip.name = testData.Name();
					testSkip.todo = false;

					for (Reporter* reporter: testContext.reporters)
					{
						reporter->Signal(testSkip);
					}
					success = true;
				}
			};

			// Start the recursive test execution
			std::visit(testExecutor, testData.Variant());

			// Get the output from the test
			TestContext::OutputData testOutput = testContext.Output(requirements);

			if (not testOutput.success)
			{
				success = false;
			}
		}

		event::TestEnd testEnd;
		testEnd.name     = testData.Name();
		testEnd.duration = duration;

		for (Reporter* reporter: testContext.reporters)
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

		for (Reporter* reporter: suiteContext.reporters)
		{
			reporter->Signal(suiteBegin);
		}

		metric::Duration duration;

		{
			metric::Timer timer(duration);

			// Fixture lifetime should be for the whole suite
			Fixture fixture = suiteContext.CreateFixture();

			auto executor = Overload{
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
					suiteContext.logger,
					filter,
					suiteContext.dryRun);

				testSuccess = ProcessTest(runner, view, testContext);

				if (not testSuccess)
				{
					success = false;
				}
			}
		}

		event::SuiteEnd end;
		end.name     = suite.Name();
		end.duration = duration;

		for (Reporter* reporter: suiteContext.reporters)
		{
			reporter->Signal(end);
		}

		return success;
	}

	export auto Execute(
		const ExecuteParameters& parameters,
		Runner& runner,
		std::span<Reporter*> reporters = {},
		const log::Logger& logger      = log::RootLogger()) -> ExecuteResult
	{
		// Break down the filter into individual views
		auto splitData = parameters.filter | std::ranges::views::split('.') |
		                 std::ranges::views::transform(
			                 [](auto&& str)
			                 {
				                 return std::string_view(str.data(), std::ranges::distance(str));
			                 });

		std::vector<std::string_view> filterData = std::ranges::to<std::vector>(splitData);

		const std::span filterViews = filterData;

		bool success = true;

		const std::string threadName = std::format("{}", std::this_thread::get_id());
		const std::uint64_t seed     = std::random_device()();

		log::Logger threadLogger = logger.CreateLogger(threadName);

		std::vector<Reporter*> resolvedReporters;
		std::vector<std::unique_ptr<Reporter>> reporterStorage;

		if (reporters.empty())
		{
			auto registrars = ReporterRegistry::Registrars();

			resolvedReporters.reserve(reporters.size());
			reporterStorage.reserve(reporters.size());

			std::ranges::for_each(
				registrars,
				[&](const auto& registrar)
				{
					reporterStorage.push_back(registrar->Create(threadLogger));
					resolvedReporters.push_back(reporterStorage.back().get());
				});
		}
		else
		{
			// An extra copy is done
			resolvedReporters = reporters | std::ranges::to<std::vector>();
		}

		const event::Initialize initialize(seed, parameters.header);
		for (Reporter* reporter: resolvedReporters)
		{
			reporter->Signal(initialize);
		}

		metric::Duration duration;
		{
			metric::Timer timer(duration);
			for (const SuiteData& suite: GetSuites())
			{
				SuiteContext suiteContext(
					resolvedReporters,
					threadLogger,
					parameters.applicationName,
					suite.Name(),
					filterViews,
					parameters.dryRun);

				success |= ProcessSuite(runner, suite, suiteContext);
			}
		}

		event::Summary summary;
		summary.duration = duration;

		for (Reporter* reporter: resolvedReporters)
		{
			reporter->Signal(summary);
		}

		// Manually clear for static analysis
		resolvedReporters.clear();
		reporterStorage.clear();

		return ExecuteResult(success);
	}
}
