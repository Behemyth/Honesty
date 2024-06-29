import std;
import synodic.honesty.test;
import synodic.honesty.log;

using namespace synodic::honesty::test::literals;

namespace
{
	synodic::honesty::test::Suite loggerSuite(
		"logger",
		[]() -> synodic::honesty::test::Generator
		{
			co_yield "parent_sink"_test = [](const synodic::honesty::test::Requirements& requirements)
			{
				const synodic::honesty::log::Logger& root = synodic::honesty::log::RootLogger();
				synodic::honesty::log::Logger parent = root.CreateLogger("parent");
				synodic::honesty::log::Logger child = parent.CreateLogger("child");

				requirements.Assert(child.Parent());
				requirements.Expect(parent == *child.Parent());

				synodic::honesty::log::RingBuffer<std::mutex> sink;

				// TODO: Compare stuff
			};
		});

	synodic::honesty::test::SuiteRegistrar _(loggerSuite);
}
