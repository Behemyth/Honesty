
export module synodic.honesty.test.entry;

import synodic.honesty.test;
import synodic.honesty.test.runner;
import synodic.honesty.test.backend;

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

	std::expected<void, TestResultCode> entry()
	{
		Registry registry;
		runner::single_threaded runner;

		auto data = registry.GetDefaultData();

		// Copy the direct registry data into the default runner
		runner.Default(data);

		return {};
	}
}
