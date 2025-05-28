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

				constexpr honesty::trace::TracerConfiguration config(true);

				// None
				constexpr auto builderNone = honesty::trace::ProviderBuilder<ProviderTypeNone>(true);

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
			};

			co_yield "validate"_test = [](const Requirements& requirements)
			{
				enum class ProviderType
				{
					BASE,
					SECOND,
					COUNT
				};

				constexpr honesty::trace::TracerConfiguration enabledConfig(true);
				constexpr honesty::trace::TracerConfiguration disabledConfig(false);

				// Provider with BASE enabled, SECOND disabled
				constexpr auto builder = honesty::trace::ProviderBuilder<ProviderType>(true)
				                         .AddConfiguration<ProviderType::BASE>(enabledConfig)
				                         .AddConfiguration<ProviderType::SECOND>(disabledConfig);

				constexpr auto provider = builder.Build();

				requirements.Expect(provider.IsTracerEnabled(ProviderType::BASE));
				requirements.Expect(!provider.IsTracerEnabled(ProviderType::SECOND));
			};

			co_yield "get"_test = [](const Requirements& requirements)
			{
				enum class ProviderType
				{
					BASE,
					SECOND,
					COUNT
				};

				constexpr honesty::trace::TracerConfiguration baseConfig(true);
				constexpr honesty::trace::TracerConfiguration secondConfig(false);

				constexpr auto builder = honesty::trace::ProviderBuilder<ProviderType>(true)
				                         .AddConfiguration<ProviderType::BASE>(baseConfig)
				                         .AddConfiguration<ProviderType::SECOND>(secondConfig);

				constexpr auto provider = builder.Build();

				const auto& baseTracer   = provider.Get(ProviderType::BASE);
				const auto& secondTracer = provider.Get(ProviderType::SECOND);

				requirements.ExpectEquals(baseTracer.GetConfiguration().enabled, true);
				requirements.ExpectEquals(secondTracer.GetConfiguration().enabled, false);
			};

			co_yield TODO / "global"_test = [](const Requirements& requirements)
			{
				//const honesty::trace::Tracer& tracer = honesty::trace::GetTracer();
			};
		}
		);
	SuiteRegistrar _(SUITE);
}
