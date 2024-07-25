import std;
import synodic.honesty.test;

using namespace synodic::honesty::test::literals;

namespace
{
	synodic::honesty::test::Suite SUITE(
		"context",
		[]() -> synodic::honesty::test::Generator
		{
			co_yield "traits"_test = [](const synodic::honesty::test::Requirements& requirements)
			{

			};
		});

	synodic::honesty::test::SuiteRegistrar _(SUITE);
}
