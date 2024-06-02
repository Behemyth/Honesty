import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	auto runnerGenerator = []()-> Generator
	{
		co_yield "declare default"_test = [](const Requirements& requirements)
		{
		};
	};
	Suite suite("runner", runnerGenerator);
	SuiteRegistrar _(suite);
}
