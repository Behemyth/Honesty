import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"fixture",
		[](const Fixture& fixture) -> Generator
		{
			co_yield "log"_test = [&](const Requirements& requirements)
			{
				std::stringstream stream;
				const TempLogger logger = fixture.TempLog(stream);

				requirements.Expect(logger.HasSink());
			};
		});
	SuiteRegistrar _(SUITE);
}
