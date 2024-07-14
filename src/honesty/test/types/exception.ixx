export module synodic.honesty.test.types:exception;

import std;

namespace synodic::honesty::test
{
	export class AssertException final : public std::exception
	{
	public:
		using std::exception::exception;
	};
}
