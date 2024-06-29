import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"test_log",
		[]() -> Generator
		{
			co_yield "tag"_test = [](const Requirements& requirements)
			{
				//requirements.Expect(false) << "This outputs only on error";
				//requirements.Expect(false).ErrorMessage = "";

				//requirements.Expect(true) << "This outputs only on error";
				//requirements.Expect(true).ErrorMessage = "";

				// Tests callable for error message
				//requirements.Expect(false) << [](){};
			};

			// TODO: Do we support error descriptions on tests?
			co_yield "test"_test = [](const Requirements& requirements)
			{
			};
		});
	SuiteRegistrar _(SUITE);
}
