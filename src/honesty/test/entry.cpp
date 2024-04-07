import synodic.honesty.test;
import std;

int main(int argc, char* argv[])
{
	// Convert all the input arguments to modern types.
	// TODO: Reflection parsing, removing the runtime overhead of converting to std::string_view
	auto arguments = std::span(argv, argc) | std::views::transform(
		                 [](const char* v)
		                 {
			                 // strlen - linear search under the hood
			                 return std::string_view(v);
		                 });

	try
	{
		synodic::honesty::test::logger::Console logger;
		synodic::honesty::test::reporter::Console reporter(logger);
		synodic::honesty::test::runner::Local runner;

		synodic::honesty::test::Interface interface(reporter, runner);

		if (std::ranges::contains(arguments, "list-tests"))
		{
			synodic::honesty::test::parameter::List parameters;

			interface.List(parameters);
		}
		else
		{
			synodic::honesty::test::parameter::Execute parameters;
			interface.Execute(parameters);
		}
		return 0;
	}
	catch (const std::invalid_argument& exception)
	{
		std::println("Error: {0}", exception.what());

		return 1;
	}
}
