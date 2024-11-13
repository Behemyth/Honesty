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

	synodic::honesty::log::Console sink;

	synodic::honesty::test::StandardRunner runner();
	synodic::honesty::test::StandardReporter reporter();
	const synodic::honesty::test::api::ExecuteParameters configuration(
		"honesty",
		"",
		runner,
		reporters,
		false,
		header,
		logger);
	synodic::honesty::test::api::ExecuteResult result = Execute(configuration);

	return 0;
}
