export module synodic.honesty.test.backend:tracer;

import synodic.honesty.trace;

namespace honesty::test
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
}
