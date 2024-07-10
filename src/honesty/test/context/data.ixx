export module synodic.honesty.test.context:data;

import std;

import :types;

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
