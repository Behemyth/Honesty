import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	Suite exceptionSuite(
		"exception",
		[]() -> generator<TestBase>
		{
			co_yield "exception"_test = []
			{
				expect_throw<std::runtime_error>(
					[]()
					{
						throw std::runtime_error {"Test exception"};
					});

				expect_not_throw(
					[]()
					{

					});
			};
		});

	bool registered = Register(exceptionSuite);
}
