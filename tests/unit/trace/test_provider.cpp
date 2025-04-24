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

			co_yield "validate"_test = [](const Requirements& requirements)
			{
				constexpr honesty::trace::Provider three("one", "two", "three");

				static_assert(three.ValidateName("three"));
				static_assert(!three.ValidateName("four"));
			};

		});
	SuiteRegistrar _(SUITE);
}
