import synodic.honesty.test;
import synodic.honesty.test.reporter.console;
import synodic.honesty.test.runner.local;
import synodic.honesty.test.logger.console;
import std;

int main(int argc, char* argv[])
{
	// Convert all the input arguments to modern types.
	// TODO: Reflection parsing, removing the runtime overhead of converting to std::string_view
	std::span<std::string_view> arguments = std::span(argv, argc) | std::views::transform(
		                                        [](const char* v)
		                                        {
			                                        // strlen - linear search under the hood
			                                        return std::string_view(v);
		                                        });

	synodic::honesty::test::Instance::Configuration configuration;
	synodic::honesty::test::Instance instance(configuration, arguments);

	return 0;
}
