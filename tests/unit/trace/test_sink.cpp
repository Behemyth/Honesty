import std;
import synodic.honesty.test;
import synodic.honesty.trace;

using namespace honesty::test;
using namespace honesty::test::literals;

namespace
{
	Suite SUITE(
		"sink",
		[]() -> Generator
		{
			co_yield "log_sink_export"_test = [](const Requirements& requirements)
			{
				std::ostringstream oss;
				honesty::trace::LogSink sink(oss);

				honesty::trace::SpanData data;
				data.name = "test_operation";
				data.startTime = honesty::trace::SpanData{}.startTime; // Use default time
				data.endTime = data.startTime + std::chrono::milliseconds(100);
				data.status = honesty::trace::SpanStatus::OK;

				std::array spans{data};
				sink.Export(spans);

				const std::string output = oss.str();
				requirements.Expect(output.contains("test_operation"));
				requirements.Expect(output.contains("OK"));
			};

			co_yield "log_sink_with_attributes"_test = [](const Requirements& requirements)
			{
				std::ostringstream oss;
				honesty::trace::LogSink sink(oss);

				honesty::trace::SpanData data;
				data.name = "attributed_operation";
				data.startTime = honesty::trace::SpanData{}.startTime;
				data.endTime = data.startTime + std::chrono::milliseconds(50);
				data.status = honesty::trace::SpanStatus::ERROR;
				data.statusMessage = "failed";
				data.AddAttribute("http.method", std::string_view{"GET"});
				data.AddAttribute("http.status", std::int64_t{200});

				std::array spans{data};
				sink.Export(spans);

				const std::string output = oss.str();
				requirements.Expect(output.contains("attributed_operation"));
				requirements.Expect(output.contains("ERROR"));
				requirements.Expect(output.contains("failed"));
				requirements.Expect(output.contains("http.method"));
			};

			co_yield "scatter_sink"_test = [](const Requirements& requirements)
			{
				std::ostringstream oss1, oss2;
				honesty::trace::LogSink sink1(oss1);
				honesty::trace::LogSink sink2(oss2);

				honesty::trace::Scatter scatter;
				scatter.AddSink(&sink1);
				scatter.AddSink(&sink2);

				requirements.ExpectEquals(scatter.Size(), std::size_t{2});

				honesty::trace::SpanData data;
				data.name = "scattered_span";
				data.startTime = honesty::trace::SpanData{}.startTime;
				data.endTime = data.startTime;
				data.status = honesty::trace::SpanStatus::OK;

				std::array spans{data};
				scatter.Export(spans);

				// Both sinks should have received the span
				requirements.Expect(oss1.str().contains("scattered_span"));
				requirements.Expect(oss2.str().contains("scattered_span"));
			};

			co_yield "fixed_scatter_sink"_test = [](const Requirements& requirements)
			{
				std::ostringstream oss1, oss2, oss3;
				honesty::trace::LogSink sink1(oss1);
				honesty::trace::LogSink sink2(oss2);
				honesty::trace::LogSink sink3(oss3);

				honesty::trace::FixedScatter<2> scatter;
				scatter.AddSink(&sink1);
				scatter.AddSink(&sink2);
				scatter.AddSink(&sink3); // Should be ignored (at capacity)

				requirements.ExpectEquals(scatter.Size(), std::size_t{2});

				honesty::trace::SpanData data;
				data.name = "fixed_scattered";
				data.startTime = honesty::trace::SpanData{}.startTime;
				data.endTime = data.startTime;

				std::array spans{data};
				scatter.Export(spans);

				// Only first two sinks should have received
				requirements.Expect(oss1.str().contains("fixed_scattered"));
				requirements.Expect(oss2.str().contains("fixed_scattered"));
				requirements.Expect(oss3.str().empty());
			};

			co_yield "scatter_remove_sink"_test = [](const Requirements& requirements)
			{
				std::ostringstream oss1, oss2;
				honesty::trace::LogSink sink1(oss1);
				honesty::trace::LogSink sink2(oss2);

				honesty::trace::Scatter scatter;
				scatter.AddSink(&sink1);
				scatter.AddSink(&sink2);
				scatter.RemoveSink(&sink1);

				requirements.ExpectEquals(scatter.Size(), std::size_t{1});

				honesty::trace::SpanData data;
				data.name = "after_remove";
				data.startTime = honesty::trace::SpanData{}.startTime;
				data.endTime = data.startTime;

				std::array spans{data};
				scatter.Export(spans);

				// Only sink2 should have received
				requirements.Expect(oss1.str().empty());
				requirements.Expect(oss2.str().contains("after_remove"));
			};
		});
	SuiteRegistrar _(SUITE);
}
