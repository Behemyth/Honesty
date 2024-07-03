import std;
import synodic.honesty.test;
import synodic.honesty.test.mock;
import synodic.honesty.log;
;
using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"command",
		[]() -> Generator
		{
			co_return;
		});
	SuiteRegistrar _(SUITE);
}
