import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite exceptionSuite(
		"exception",
		[]()
		{
			"exception"_test = []
			{
				ExpectThrow<std::runtime_error>(
					[]()
					{
						throw std::runtime_error{"Test exception"};
					});

				ExpectNotThrow(
					[]()
					{
					});
			};
		});

	bool registered = RegisterSuite(exceptionSuite);
}
