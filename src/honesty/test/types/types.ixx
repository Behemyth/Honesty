export module synodic.honesty.test.types;

import std;

namespace synodic::honesty::test
{

	export class AssertException final : public std::exception
	{
	public:
		using std::exception::exception;
	};
}
