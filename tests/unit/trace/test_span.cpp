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

		return provider.Get();
	}

	Suite SUITE(
		"span",
		[]() -> Generator
		{
			auto& tracer = GetTracer();

			co_yield "span"_test = [&](const Requirements& requirements)
			{
					honesty::Span span = tracer.CreateSpan("span");
			};
		});
	SuiteRegistrar _(SUITE);
}
