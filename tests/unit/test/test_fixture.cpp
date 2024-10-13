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
				const std::stringstream& stream = fixture.AttachListener();

				requirements.Expect(stream.view().empty());

				constexpr std::string_view expectedString = "This should be visible to the listener";

				requirements.Expect(not stream.view().empty(), expectedString);
				requirements.ExpectEquals(stream.view(), expectedString);
			};
		});
	SuiteRegistrar _(SUITE);
}
