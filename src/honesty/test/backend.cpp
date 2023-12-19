module synodic.honesty.test.backend;

import counter;

namespace
{
	std::vector<synodic::honesty::suite_data>& GetSuiteStorage()
	{
		static std::vector<synodic::honesty::suite_data> suites;
		return suites;
	}
}

namespace synodic::honesty
{
	std::span<suite_data> Suites()
	{
		return GetSuiteStorage();
	}
}
