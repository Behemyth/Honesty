import std;
import synodic.honesty.test;
import synodic.honesty.trace;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	Suite SUITE(
		"provider",
		[]() -> Generator
		{
			co_yield "construction"_test = [](const Requirements& requirements)
			{
				honesty::trace::Provider one("one");
				honesty::trace::Provider two("one", "two");
				honesty::trace::Provider three("one", "two", "three");
			};

			co_yield "get"_test = [](const Requirements& requirements)
			{
				constexpr honesty::trace::Provider three("one", "two", "three");

				const honesty::trace::Tracer& success = three.Get("three");

				const honesty::trace::Tracer& failure = three.Get("four");
			};

		});
	SuiteRegistrar _(SUITE);
}
