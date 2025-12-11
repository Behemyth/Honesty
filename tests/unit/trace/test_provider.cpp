import std;
import synodic.honesty.test;
import synodic.honesty.trace;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	// Define enums and configs at namespace scope to avoid nested initializer issues
	enum class SingleProviderType
	{
		BASE,
		COUNT
	};

	enum class DualProviderType
	{
		BASE,
		SECOND,
		COUNT
	};

	Suite SUITE(
		"provider",
		[]() -> Generator
		{
			co_yield "size"_test = [](const Requirements& requirements)
			{
				constexpr honesty::trace::TracerConfiguration config{true};

				// Test that Size() returns correct value
				honesty::trace::Tracer<config> tracer;
				requirements.Expect(tracer.GetConfiguration().enabled);
			};

			co_yield "tracer_configuration"_test = [](const Requirements& requirements)
			{
				constexpr honesty::trace::TracerConfiguration enabledConfig{true};
				constexpr honesty::trace::TracerConfiguration disabledConfig{false};

				honesty::trace::Tracer<enabledConfig> enabled;
				honesty::trace::Tracer<disabledConfig> disabled;

				requirements.Expect(enabled.GetConfiguration().enabled);
				requirements.Expect(!disabled.GetConfiguration().enabled);
			};
		}
		);
	SuiteRegistrar _(SUITE);
}
