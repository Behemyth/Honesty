import synodic.honesty.test;
import std;

namespace synodic::honesty
{
	enum class Mode
	{
		EXECUTE
	};
}

int main(int argc, char* argv[])
{
	auto arguments = std::span(argv, argc) | std::views::transform(
								[](const char* v)
								{
									// strlen linear search under the hood
									return std::string_view(v);
								});

	constexpr synodic::honesty::Mode mode = synodic::honesty::Mode::EXECUTE;

	if (std::ranges::contains(arguments, "list-tests"))
	{
		
	}

	try
	{
		switch (mode)
		{
			case synodic::honesty::Mode::EXECUTE :
			{
				synodic::honesty::test::logger::Console logger;
				synodic::honesty::test::reporter::Console reporter(logger);
				synodic::honesty::test::runner::Local runner;

				auto result = entry(reporter, runner);
				break;
			}
			default :
			{
				std::terminate();
			}
		}

		return 0;
	}
	catch (const std::invalid_argument& exception)
	{
		std::println("Error: {0}", exception.what());

		return 1;
	}
}
