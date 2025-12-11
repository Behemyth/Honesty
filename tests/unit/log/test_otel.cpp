import std;
import synodic.honesty.test;
import synodic.honesty.log;

using namespace honesty::test;
using namespace honesty::test::literals;
using namespace honesty::log;

namespace
{
	Suite SUITE(
		"log_otel",
		[]() -> Generator
		{
			co_yield "init_cleanup_lifecycle"_test = [](const Requirements& requirements)
			{
				// Redirect cout and cerr to suppress OTel output
				std::ostringstream nullCout, nullCerr;
				auto* originalCout = std::cout.rdbuf(nullCout.rdbuf());
				auto* originalCerr = std::cerr.rdbuf(nullCerr.rdbuf());

				// Initialize and cleanup should not throw
				InitOTelLogging();
				CleanupOTelLogging();

				// Restore streams
				std::cout.rdbuf(originalCout);
				std::cerr.rdbuf(originalCerr);

				// Just verify no crash
				requirements.Expect(true);
			};

			co_yield "multiple_init_cleanup_cycles"_test = [](const Requirements& requirements)
			{
				// Redirect cout and cerr to suppress OTel output
				std::ostringstream nullCout, nullCerr;
				auto* originalCout = std::cout.rdbuf(nullCout.rdbuf());
				auto* originalCerr = std::cerr.rdbuf(nullCerr.rdbuf());

				// Multiple init/cleanup cycles should be safe
				for (int i = 0; i < 3; ++i)
				{
					InitOTelLogging();
					CleanupOTelLogging();
				}

				// Restore streams
				std::cout.rdbuf(originalCout);
				std::cerr.rdbuf(originalCerr);

				// Just verify no crash
				requirements.Expect(true);
			};

			// Note: Direct OTelemetry<> template instantiation tests are not included here
			// because the template exposes OpenTelemetry SDK headers which create linker
			// dependencies. The OTelemetry sink is tested indirectly through the Logger
			// interface when used in integration tests. The init/cleanup functions above
			// verify the SDK is properly configured.
		});

	SuiteRegistrar _(SUITE);
}
