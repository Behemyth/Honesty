import std;
import synodic.honesty.test;
import synodic.honesty.log;

using namespace synodic::honesty::test::literals;

namespace
{
	synodic::honesty::test::Suite loggerSuite(
		"logger",
		[]() -> synodic::honesty::test::generator<synodic::honesty::test::TestBase>
		{
			co_yield "Parent Sink"_test = []()
			{
				synodic::honesty::log::Logger& parent = synodic::honesty::log::GetLogger("parent");
				synodic::honesty::log::Logger& child = synodic::honesty::log::GetLogger("parent.child");

				synodic::honesty::log::RingBuffer<std::mutex> sink;

				parent.AddSink(&sink);
			};
		});

	bool registered = Register(loggerSuite);
}
