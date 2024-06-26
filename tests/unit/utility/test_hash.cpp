import std;
import synodic.honesty.test;
import synodic.honesty.utility;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite SUITE(
		"hash",
		[]() -> Generator
		{
			co_return;
		});

	SuiteRegistrar _(SUITE);
}
