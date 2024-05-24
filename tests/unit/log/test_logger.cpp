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
			co_yield "Parent Sink"_test = []()
			{
				const synodic::honesty::log::Logger& root = synodic::honesty::log::RootLogger();
				synodic::honesty::log::Logger parent = root.CreateLogger("parent");
				synodic::honesty::log::Logger child = parent.CreateLogger("child");

				synodic::honesty::test::Assert(child.Parent());
				synodic::honesty::test::ExpectEquals(parent, *child.Parent());

				synodic::honesty::log::RingBuffer<std::mutex> sink;
			};
		});

	synodic::honesty::test::SuiteRegistrar _(loggerSuite);
}
