export module synodic.honesty.test.backend:storage;

import std;

import :data;

import inplace_vector;

namespace
{
	constinit std::inplace_vector<synodic::honesty::test::SuiteData, 50> SUITES;
}

namespace synodic::honesty::test
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
