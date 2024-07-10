export module synodic.honesty.utility:exception;

import std;

namespace synodic::honesty::utility
{

	export class NotImplemented final : public std::logic_error
	{
	public:
		NotImplemented() :
			std::logic_error("Function is not implemented") {};
	};

}
