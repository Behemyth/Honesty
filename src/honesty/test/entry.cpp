import synodic.honesty.test;
import std;

int main(const int argc, const char* argv[])
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

	synodic::honesty::test::Instance::Configuration configuration;
	synodic::honesty::test::Instance instance(configuration, arguments);

	instance.Execute();

	return 0;
}
