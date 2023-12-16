import synodic.honesty.test.entry;
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
	// TODO: Parse inputs

	constexpr synodic::honesty::Mode mode = synodic::honesty::Mode::EXECUTE;

	try
	{
		switch (mode)
		{
			case synodic::honesty::Mode::EXECUTE:
			{
				auto result = synodic::honesty::entry();
				break;
			}
			default:
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
