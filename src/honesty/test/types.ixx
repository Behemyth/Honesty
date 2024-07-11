export module synodic.honesty.test:types;

import std;
import function_ref;
import synodic.honesty.log;

namespace synodic::honesty::test
{
	export enum class ListOutputType : std::uint8_t
	{
		LOG,
		JSON
	};

}
