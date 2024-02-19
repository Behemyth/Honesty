module synodic.honesty.test.backend:reporter;

import :reporter;
import std;

namespace synodic::honesty
{
	Events::Events(std::span<Reporter* const> reporters) :
		reporters_(reporters)

	{
	}
}
