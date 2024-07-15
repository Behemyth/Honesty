import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite exceptionSuite(
		"exception",
		[]()-> Generator
		{
			co_yield "exception"_test = [](const Requirements& requirements)
			{
				requirements.ExpectThrow<std::runtime_error>(
					[]()
					{
						throw std::runtime_error("Test exception");
					});

				requirements.ExpectNotThrow(
					[]()
					{
					});
			};
		});

	SuiteRegistrar _(exceptionSuite);
}
