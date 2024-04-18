import synodic.honesty.test;

// TODO: Report compile-time tests

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite expectationSuite(
		"expectation",
		[]() -> generator<TestBase>
		{
			co_yield "expectation"_test = []
			{
				// TODO: except can fail and continue
				// TODO: assert stops when it fails
			};
		});

	bool registered = Register(expectationSuite);
}
