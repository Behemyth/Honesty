
export module synodic.honesty.test.entry;

import std;

namespace synodic::honesty
{
}

export namespace synodic::honesty
{
	enum class TestResultCode : std::uint8_t
	{
		FAIL
	};

	std::expected<void, TestResultCode> entry();
}
