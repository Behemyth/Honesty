import std;
import synodic.honesty.test;
import synodic.honesty.test.api;
import synodic.honesty.log;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	Suite SUITE(
		"reporter",
		[](Fixture& fixture) -> Generator
		{
			co_yield "empty_description"_test = [&](const Requirements& requirements)
			{
				std::stringstream stream;
				honesty::log::OStream<std::mutex> sink(stream);
				sink.SetLevel(honesty::log::LevelType::INFO);

				const honesty::log::Logger& root = honesty::log::RootLogger();
				honesty::log::Logger logger      = root.CreateLogger("reporter_test");
				logger.SetSink(&sink);

				StandardReporter reporter(logger);

				// Simulate the full event sequence for a test failure with an empty description
				const event::SuiteBegin suiteBeginEvent{"suite_name"};
				reporter.Signal(suiteBeginEvent);

				const event::TestBegin testBeginEvent("test_name", ExpectedAssertOutcome::PASS);
				reporter.Signal(testBeginEvent);

				const event::AssertionFail failEvent(std::source_location::current(), "", false);
				reporter.Signal(failEvent);

				const event::TestEnd testEndEvent{"test_name", {}};
				reporter.Signal(testEndEvent);

				const event::SuiteEnd suiteEndEvent{"suite_name", {}};
				reporter.Signal(suiteEndEvent);

				requirements.Expect(
					not stream.view().contains("Description:"),
					"Empty descriptions should not be displayed in test output");
			};

			co_yield "provided_description"_test = [&](const Requirements& requirements)
			{
				std::stringstream stream;
				honesty::log::OStream<std::mutex> sink(stream);
				sink.SetLevel(honesty::log::LevelType::INFO);

				const honesty::log::Logger& root = honesty::log::RootLogger();
				honesty::log::Logger logger      = root.CreateLogger("reporter_test");
				logger.SetSink(&sink);

				StandardReporter reporter(logger);

				// Simulate the full event sequence for a test failure with a description
				const event::SuiteBegin suiteBeginEvent{"suite_name"};
				reporter.Signal(suiteBeginEvent);

				const event::TestBegin testBeginEvent("test_name", ExpectedAssertOutcome::PASS);
				reporter.Signal(testBeginEvent);

				const event::AssertionFail failEvent(
					std::source_location::current(),
					"Custom error message",
					false);
				reporter.Signal(failEvent);

				const event::TestEnd testEndEvent{"test_name", {}};
				reporter.Signal(testEndEvent);

				const event::SuiteEnd suiteEndEvent{"suite_name", {}};
				reporter.Signal(suiteEndEvent);

				requirements.Expect(
					stream.view().contains("Description:"),
					"Provided descriptions should be displayed in test output");

				requirements.Expect(
					stream.view().contains("Custom error message"),
					"The custom description text should appear in output");
			};
		});

	SuiteRegistrar _(SUITE);
}
