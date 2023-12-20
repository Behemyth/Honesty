
export module synodic.honesty.test.entry;

import std;

import synodic.honesty.test;
import synodic.honesty.test.backend;
import synodic.honesty.test.runner;
import synodic.honesty.test.logger;
import synodic.honesty.test.reporter;

namespace synodic::honesty
{
}

export namespace synodic::honesty
{
	enum class TestResultCode : std::uint8_t
	{
		FAIL
	};

	template<
		is_runner Runner	 = runner::single_threaded,
		is_logger Logger	 = logger::StandardOut,
		is_reporter Reporter = reporter::StandardOut>
	std::expected<void, TestResultCode>
		entry(const Runner& runner = Runner(), const Logger& logger = Logger(), const Reporter& reporter = Reporter())
	{
		auto suites = Registry::GenerateSuites();

		runner.run(suites);

		return {};
	}
}
