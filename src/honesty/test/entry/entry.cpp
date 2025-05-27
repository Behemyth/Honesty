import std;

import synodic.honesty.log;
import synodic.honesty.test.api;
import synodic.honesty.test;

auto main(const int argc, const char* argv[]) -> int
{
	// Convert all the input arguments to views
	auto view = std::span(argv, argc) | std::views::transform(
		            [](const char* value) -> std::string_view
		            {
			            // strlen - linear search under the hood
			            return std::string_view(value);
		            });

	// Convert the lazy view to contiguous data
	auto arguments = std::ranges::to<std::vector>(view);

	// TODO: Use arguments to configure the test runner

	honesty::log::Console sink;

	honesty::log::Logger logger = honesty::log::RootLogger().CreateLogger("main");

	logger.SetSink(&sink);
	logger.SetLevel(honesty::log::LevelType::INFO);

	honesty::test::StandardRunner runner(logger);
	
	honesty::test::ReporterRegistrar<honesty::test::StandardReporter> standardReporter;

	const honesty::test::api::ExecuteParameters
		configuration("honesty", "", runner, false, {}, logger);

	const honesty::test::api::ExecuteResult result = Execute(configuration);

	return !result.success;
}
