import std;
import synodic.honesty.test;
import synodic.honesty.utility;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite hashSuite(
		"hash",
		[]() -> Generator
		{
			co_return;
		});

	SuiteRegistrar _(hashSuite);
}
