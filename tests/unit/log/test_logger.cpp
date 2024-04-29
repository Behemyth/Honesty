import std;
import synodic.honesty.test;
import synodic.honesty.log;

using namespace synodic::honesty::test::literals;

namespace
{
	synodic::honesty::test::Suite loggerSuite(
		"logger",
		[]()-> synodic::honesty::test::Generator
		{
			co_yield "Parent Sink"_test = []()
			{
				synodic::honesty::log::DynamicLogger& parent = synodic::honesty::log::GetLogger("parent");
				synodic::honesty::log::DynamicLogger& child = synodic::honesty::log::GetLogger("parent.child");

				synodic::honesty::test::Assert(child.Parent());
				synodic::honesty::test::ExpectEquals(parent, *child.Parent());

				synodic::honesty::log::RingBuffer<std::mutex> sink;

				parent.AddSink(&sink);
			};
		});

	bool registered = RegisterSuite(loggerSuite);
}
