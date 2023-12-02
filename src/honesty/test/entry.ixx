
export module synodic.honesty.test:entry;

import std;

import  synodic.honesty.test.runner;

import synodic.honesty.test.logger;
import synodic.honesty.test.reporter;

export namespace synodic::honesty
{
	enum class TestResultCode
	{
		FAIL
	};

	template<is_logger Logger = logger::StandardOut, is_reporter Reporter = reporter::StandardOut>
	std::expected<void, TestResultCode> entry(Logger& logger, Reporter& reporter)
	{
		Runner runner;

		runner.Run();

		//return std::unexpected(TestResultCode::FAIL);

		return {};
	}
}
