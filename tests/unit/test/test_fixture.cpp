import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"fixture",
		[](Fixture& fixture) -> Generator
		{
			co_yield "log"_test = [&](const Requirements& requirements)
			{
				std::stringstream stream;
				const TempLogger logger = fixture.TempLog(stream);

				requirements.Expect(logger.HasSink());

				constexpr std::string_view message("test");
				logger.Info(message);

				requirements.ExpectEquals(stream.str(), message);
			};

			co_yield "log_listener"_test = [&](const Requirements& requirements)
			{
				const std::stringstream& stream = fixture.AttachListener(synodic::honesty::log::LevelType::TRACE);

				requirements.Expect(
					stream.view().empty(),
					"The stream should be empty because we just started listening");
				requirements.Expect(
					stream.view().empty(),
					"The previous expectation passed, so the log stream should be empty");

				requirements.ExpectThrow<std::runtime_error>(
					[]()
					{
						throw std::runtime_error("Test exception");
					});

				requirements.Expect(
					not stream.view().empty(),
					"An exception was thrown and should have been written to the log");
			};
		});
	SuiteRegistrar _(SUITE);
}
