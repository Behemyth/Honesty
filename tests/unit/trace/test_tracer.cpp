import std;
import synodic.honesty.test;
import synodic.honesty.trace;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	consteval const auto& GetTracer()
	{
		enum class ProviderType
		{
			BASE,
			COUNT
		};

		constexpr honesty::trace::TracerConfiguration baseConfig(true);

		constexpr auto builder = honesty::trace::ProviderBuilder<ProviderType>()
			.AddConfiguration<ProviderType::BASE, baseConfig>();

		constexpr auto provider = builder.Build();

		return provider.Get<ProviderType::BASE>();
	}

	Suite SUITE(
		"tracer",
		[]() -> Generator
		{
			co_yield "is_trivially_constructible"_test = [](const Requirements& requirements)
			{
				constexpr auto configuration = honesty::trace::TracerConfiguration(true);

				static_assert(
					std::is_trivially_constructible_v<honesty::trace::Tracer<configuration>>);
			};

			co_yield "span"_test = [](const Requirements& requirements)
			{
				const auto& tracer = GetTracer();
				{
					auto span = tracer.CreateSpan("test_span");
				}

				requirements.Expect(tracer.GetConfiguration().enabled);
			};
		});
	SuiteRegistrar _(SUITE);
}
