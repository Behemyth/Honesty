
export module synodic.honesty.test.entry;

import synodic.honesty.test;
import synodic.honesty.test.runner;
import synodic.honesty.test.reporter;
import synodic.honesty.test.backend;

import std;

namespace synodic::honesty
{
}

export namespace synodic::honesty
{
	enum class TestResultCode : std::uint8_t
	{
		FAILURE
	};

	std::expected<void, TestResultCode> entry()
	{
		// TODO: Dynamic reporter and runner
		reporter::Console reporter;
		runner::Local runner;

		auto suites = Registry::GetDefaultSuites();

		// Move the direct registry data into the default runner
		runner.Submit(suites);

		runner.Run();

		return {};
	}
}
