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
				enum class ProviderTypeOne
				{
					BASE,
					COUNT
				};

				enum class ProviderTypeTwo
				{
					BASE,
					COUNT
				};

				enum class ProviderTypeThree
				{
					BASE,
					COUNT
				};

				constexpr honesty::trace::Tracer::Configuration config;

				constexpr honesty::trace::Provider<ProviderTypeOne> one(config);
				//constexpr honesty::trace::Provider two(config, config);
				//constexpr honesty::trace::Provider three(config, config, config);

				//requirements.ExpectEquals(one.Size(), 1);
				//requirements.ExpectEquals(two.Size(), 2);
				//requirements.ExpectEquals(three.Size(), 3);
			};

			co_yield "validate"_test = [](const Requirements& requirements)
			{
				/*	constexpr honesty::trace::Provider three("one", "two", "three");
	
					static_assert(three.IsTracerEnabled("three"));
					static_assert(!three.IsTracerEnabled("four"));*/
			};

			co_yield "get"_test = [](const Requirements& requirements)
			{
				/*constexpr honesty::trace::Provider provider("one", "two", "three");

				constexpr honesty::trace::Tracer one = provider.Get("one");*/
			};

			co_yield "global"_test = [](const Requirements& requirements)
			{
				//const honesty::Tracer& tracer = honesty::GetTracer("test");
			};
		});
	SuiteRegistrar _(SUITE);
}
