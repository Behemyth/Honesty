import std;
import synodic.honesty.test;
import synodic.honesty.log;

using namespace honesty::test;
using namespace honesty::test::literals;
using namespace honesty::log;

namespace
{
	Suite SUITE(
		"otel_sink",
		[]() -> Generator
		{
			co_yield "level_conversion"_test = [](const Requirements& requirements)
			{
				// Test that all log levels are defined and distinct
				requirements.Expect(LevelType::DEFER != LevelType::TRACE);
				requirements.Expect(LevelType::TRACE != LevelType::DEBUG);
				requirements.Expect(LevelType::DEBUG != LevelType::INFO);
				requirements.Expect(LevelType::INFO != LevelType::WARNING);
				requirements.Expect(LevelType::WARNING != LevelType::ERROR);
				requirements.Expect(LevelType::ERROR != LevelType::CRITICAL);
			};

			co_yield "level_ordering"_test = [](const Requirements& requirements)
			{
				// Verify severity ordering (lower value = less severe)
				requirements.Expect(static_cast<int>(LevelType::TRACE) < static_cast<int>(LevelType::DEBUG));
				requirements.Expect(static_cast<int>(LevelType::DEBUG) < static_cast<int>(LevelType::INFO));
				requirements.Expect(static_cast<int>(LevelType::INFO) < static_cast<int>(LevelType::WARNING));
				requirements.Expect(static_cast<int>(LevelType::WARNING) < static_cast<int>(LevelType::ERROR));
				requirements.Expect(static_cast<int>(LevelType::ERROR) < static_cast<int>(LevelType::CRITICAL));
			};
		});

	SuiteRegistrar _(SUITE);
}
