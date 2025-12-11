import std;
import synodic.honesty.test;
import synodic.honesty.trace;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	// Use simple Tracer directly to avoid MSVC compiler limits with nested initializers
	constexpr honesty::trace::TracerConfiguration enabledConfig{true};
	constexpr honesty::trace::TracerConfiguration disabledConfig{false};
	honesty::trace::Tracer<enabledConfig> enabledTracer;
	honesty::trace::Tracer<disabledConfig> disabledTracer;

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

			co_yield "enabled_tracer_creates_span"_test = [](const Requirements& requirements)
			{
				auto span = enabledTracer.CreateSpan("test_span");

				requirements.Expect(enabledTracer.GetConfiguration().enabled);

				// Verify span has valid data
				const auto& data = span.Data();
				requirements.ExpectEquals(data.name, std::string_view{"test_span"});
				requirements.Expect(data.traceID.IsValid());
				requirements.Expect(data.spanID.IsValid());
			};

			co_yield "disabled_tracer_creates_noop"_test = [](const Requirements& requirements)
			{
				auto span = disabledTracer.CreateSpan("should_be_noop");

				requirements.Expect(!disabledTracer.GetConfiguration().enabled);

				// NoopSpan has no data() method - it's completely elided
				static_assert(std::is_same_v<decltype(span), honesty::NoopSpan>);
			};

			co_yield "zero_cost_when_disabled"_test = [](const Requirements& requirements)
			{
				// Verify that disabled tracer has minimal size (no sink pointer)
				constexpr auto config = honesty::trace::TracerConfiguration(false);
				using DisabledTracer = honesty::trace::Tracer<config>;

				// The tracer should only contain TracerOptions when disabled
				requirements.Expect(sizeof(DisabledTracer) <= sizeof(honesty::trace::TracerOptions) + 8);
			};
		});
	SuiteRegistrar _(SUITE);
}
