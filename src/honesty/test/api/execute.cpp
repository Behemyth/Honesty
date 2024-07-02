module synodic.honesty.test:api.execute;

import std;
import synodic.honesty.log;
import :suite;
import :context;

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
	struct ExecuteParameters
	{
		explicit ExecuteParameters(
			const std::string_view applicationName,
			const std::string_view filter,
			Runner& runner,
			const std::span<std::unique_ptr<Reporter>> reporters,
			const log::Logger& logger) :
			applicationName(applicationName),
			filter(filter),
			runner(runner),
			reporters(reporters),
			logger(logger)
		{
		}

		std::string_view applicationName;
		std::string_view filter;

		std::reference_wrapper<Runner> runner;
		std::span<std::unique_ptr<Reporter>> reporters;

		std::reference_wrapper<const log::Logger> logger;
	};

	struct ExecuteResult
	{
		explicit ExecuteResult(const bool success) :
			success(success)
		{
		}

		bool success;
	};

	auto Execute(const ExecuteParameters& parameters) -> ExecuteResult
	{
		// Before start executing, we need to set up the current thread's context
		Context context(&parameters.runner.get(), parameters.reporters);

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

		for (const SuiteView& suite: GetSuites())
		{
			// Before we start, check to see if we have a filter
			if (not parameters.filter.empty())
			{
				// Check if the suite name matches the filter
				if (suite.name != filter.front())
				{
					continue;
				}

				// Move to the next filter part
				filter = filter | std::ranges::views::drop(1);
			}

			event::SuiteBegin suiteBegin;
			suiteBegin.name = suite.name;

			context.Signal(suiteBegin);

			// Fixture lifetime should be for the whole suite
			Fixture fixture(parameters.applicationName, suite.name, parameters.logger);

			auto executor = Overload{
				[&](const std::function_ref<Generator()> generator) -> Generator
				{
					return generator();
				},
				[&](const std::function_ref<Generator(Fixture&)> generator) -> Generator
				{
					return generator(fixture);
				}};

			Generator generator = std::visit(executor, suite.testGenerator);

			for (const Test& test: generator)
			{
				const TestView view(test);

				// Filter the test by name
				if (not filter.empty() and view.name != filter.front())
				{
					continue;
				}

				event::TestBegin testBegin;
				testBegin.name = view.name;

				context.Signal(testBegin);

				RequirementsBackend requirements(parameters.reporters, test.Name(), parameters.logger);

				auto testExecutor = Overload{
					[&](const std::function_ref<void(Requirements&)> testCallback) -> Generator
					{
					},
					[&](const std::function_ref<Generator(Requirements&)> testCallback) -> Generator
					{
					}};

				Generator generator = std::visit(testExecutor, view.test);

				// TODO: Run nested tests
				// context.Run(requirements, view.test);

				if (not requirements.Context().success)
				{
					success = false;
				}

				event::TestEnd testEnd;
				testEnd.name = view.name;

				context.Signal(testEnd);
			}

			event::SuiteEnd end;
			end.name = suite.name;

			context.Signal(end);
		}

		event::Summary summary;
		context.Signal(summary);

		return ExecuteResult(success);
	}
}
