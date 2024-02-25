module synodic.honesty.test.reporter:reporter;

import :reporter;
import std;

namespace synodic::honesty::test
{
	Events::Events(std::span<Reporter* const> reporters) :
		reporters_(reporters)

	{
	}
}
