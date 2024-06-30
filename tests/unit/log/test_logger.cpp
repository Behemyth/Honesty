import std;
import synodic.honesty.test;
import synodic.honesty.log;

using namespace synodic::honesty::test::literals;

namespace
{
	synodic::honesty::test::Suite SUITE(
		"logger",
		[]() -> synodic::honesty::test::Generator
		{
			co_yield "parent_sink"_test = [](const synodic::honesty::test::Requirements& requirements)
			{
				const synodic::honesty::log::Logger& root = synodic::honesty::log::RootLogger();

				std::stringstream stream;
				synodic::honesty::log::OStream<std::mutex> sink(stream);

				synodic::honesty::log::Logger parent = root.CreateLogger("parent");
				const synodic::honesty::log::Logger child = parent.CreateLogger("child");

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

	synodic::honesty::test::SuiteRegistrar _(SUITE);
}
