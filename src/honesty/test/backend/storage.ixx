export module synodic.honesty.test.backend:storage;

import std;

import :suite;
import inplace_vector;

namespace
{
	constinit std::inplace_vector<honesty::test::SuiteData, 50> SUITES;
}

namespace honesty::test
{
	export std::span<SuiteData> GetSuites()
	{
		return SUITES;
	}

	export void AddSuite(SuiteData suite)
	{
		SUITES.push_back(std::move(suite));
	}
}
