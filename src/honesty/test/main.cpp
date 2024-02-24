import synodic.honesty.test.entry;
import std;
import synodic.honesty.test;
import synodic.honesty.test.reporter;
import synodic.honesty.test.runner;
import synodic.honesty.test.logger;

namespace synodic::honesty
{
	enum class Mode
	{
		EXECUTE
	};
}

int main(int argc, char* argv[])
{
	// TODO: Parse inputs

	constexpr synodic::honesty::Mode mode = synodic::honesty::Mode::EXECUTE;

	try
	{
		switch (mode)
		{
			case synodic::honesty::Mode::EXECUTE :
			{
				synodic::honesty::logger::Console logger;
				synodic::honesty::reporter::Console<synodic::honesty::logger::Console> reporter(logger);
				synodic::honesty::runner::Local runner;

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
