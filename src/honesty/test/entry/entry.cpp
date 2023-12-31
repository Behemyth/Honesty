module synodic.honesty.test.entry;

import synodic.honesty.test.backend;
import synodic.honesty.test.runner;
import synodic.honesty.test.entry;
import std;

namespace synodic::honesty
{
	std::expected<void, TestResultCode> entry()
	{
		const auto suites = Registry::DefaultedData();

		if(!suites.empty())
		{
			runner::single_threaded runner;
		}

		runner.run();

		return {};
	}
}
