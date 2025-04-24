import std;

import synodic.honesty.log;
import synodic.honesty.test.api;

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

	honesty::log::Console sink;

	honesty::log::Logger logger = honesty::log::RootLogger().CreateLogger("main");

	logger.SetSink(&sink);
	logger.SetLevel(honesty::log::LevelType::INFO);

	honesty::test::StandardRunner runner(logger);

	std::array<std::unique_ptr<honesty::test::Reporter>, 1> reporters{
		std::make_unique<honesty::test::StandardReporter>(logger)};

	const honesty::test::api::ExecuteParameters
		configuration("honesty", "", runner, reporters, false, {}, logger);
	honesty::test::api::ExecuteResult result = Execute(configuration);

	return 0;
}
