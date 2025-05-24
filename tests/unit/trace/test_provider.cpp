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
				enum class ProviderTypeNone
				{
					COUNT
				};

				enum class ProviderTypeOne
				{
					BASE,
					COUNT
				};

				enum class ProviderTypeTwo
				{
					BASE,
					SECOND,
					COUNT
				};

				enum class ProviderTypeThree
				{
					BASE,
					SECOND,
					THIRD,
					COUNT
				};

				constexpr honesty::trace::TracerConfiguration config(true);

				// None
				constexpr auto builderNone  = honesty::trace::ProviderBuilder<ProviderTypeNone>(true);
				constexpr auto providerNone = builderNone.Build();
				requirements.ExpectEquals(providerNone.Size(), 0);

				// One
				constexpr auto builderOne = honesty::trace::ProviderBuilder<ProviderTypeOne>(true)
					.AddConfiguration<ProviderTypeOne::BASE>(config);
				constexpr auto providerOne = builderOne.Build();
				requirements.ExpectEquals(providerOne.Size(), 1);

				// Two
				constexpr auto builderTwo = honesty::trace::ProviderBuilder<ProviderTypeTwo>(true)
				                            .AddConfiguration<ProviderTypeTwo::BASE>(config)
				                            .AddConfiguration<ProviderTypeTwo::SECOND>(config);
				constexpr auto providerTwo = builderTwo.Build();
				requirements.ExpectEquals(providerTwo.Size(), 2);

				// Three
				constexpr auto builderThree = honesty::trace::ProviderBuilder<ProviderTypeThree>(true)
				                              .AddConfiguration<ProviderTypeThree::BASE>(config)
				                              .AddConfiguration<ProviderTypeThree::SECOND>(config)
				                              .AddConfiguration<ProviderTypeThree::THIRD>(config);
				constexpr auto providerThree = builderThree.Build();
				requirements.ExpectEquals(providerThree.Size(), 3);
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
		}
		);
	SuiteRegistrar _(SUITE);
}
