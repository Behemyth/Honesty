import std;
import synodic.honesty.test;
import synodic.honesty.log;

using namespace synodic::honesty::test::literals;

namespace
{
	synodic::honesty::test::Suite loggerSuite(
		"logger",
		[]()
		{
			"Parent Sink"_test = []()
			{
				synodic::honesty::log::Logger& parent = synodic::honesty::log::GetLogger("parent");
				synodic::honesty::log::Logger& child = synodic::honesty::log::GetLogger("parent.child");

				synodic::honesty::test::Assert(child.Parent());
				synodic::honesty::test::ExpectEquals(parent, *child.Parent());

				synodic::honesty::log::RingBuffer<std::mutex> sink;

				parent.AddSink(&sink);
			};
		});

	bool registered = Register(loggerSuite);
}
