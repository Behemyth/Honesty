import std;
import synodic.honesty.test;
import synodic.honesty.log;

using namespace honesty::test::literals;

namespace
{
	honesty::test::Suite SUITE(
		"logger",
		[]() -> honesty::test::Generator
		{
			co_yield "parent_sink"_test = [](const honesty::test::Requirements& requirements)
			{
				const honesty::log::Logger& root = honesty::log::RootLogger();

				std::stringstream stream;
				honesty::log::OStream<std::mutex> sink(stream);

				honesty::log::Logger parent = root.CreateLogger("parent");
				const honesty::log::Logger child = parent.CreateLogger("child");

				parent.SetSink(&sink);

				requirements.Assert(child.Parent());
				requirements.Expect(parent == *child.Parent());

				constexpr std::string_view message("message");

				child.Trace(message);

				requirements.ExpectEquals(stream.str(), message);
				stream.str("");

				child.Debug(message);

				requirements.ExpectEquals(stream.str(), message);
				stream.str("");

				child.Info(message);

				requirements.ExpectEquals(stream.str(), message);
				stream.str("");

				child.Warning(message);

				requirements.ExpectEquals(stream.str(), message);
				stream.str("");

				child.Error(message);

				requirements.ExpectEquals(stream.str(), message);
				stream.str("");

				child.Critical(message);

				requirements.ExpectEquals(stream.str(), message);
				stream.str("");
			};
		});

	honesty::test::SuiteRegistrar _(SUITE);
}
