import synodic.honesty.test;

import std;

int main(int argc, char* argv[])
{
	// Convert all the input arguments to modern types.
	std::span<std::string_view> arguments = std::span(argv, argc) | std::views::transform(
		                                        [](const char* v)
		                                        {
			                                        // strlen - linear search under the hood
			                                        return std::string_view(v);
		                                        });

	synodic::honesty::test::Instance::Configuration configuration;
	synodic::honesty::test::Instance instance(configuration, arguments);

	instance.Execute();

	return 0;
}
