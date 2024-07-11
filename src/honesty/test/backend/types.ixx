export module synodic.honesty.test.backend:types;

import std;

namespace synodic::honesty::test
{

	export class AssertException final : public std::exception
	{
	public:
		using std::exception::exception;
	};
}
