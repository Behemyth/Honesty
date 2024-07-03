export module synodic.honesty.test:api.types;

import std;

namespace synodic::honesty::test
{
	enum class TestResultCode : std::uint8_t
	{
		FAILURE
	};

	struct TestDescription
	{
		TestDescription() = default;

		std::string name;
	};

	struct SuiteDescription
	{
		SuiteDescription() = default;

		std::string name;
		std::vector<TestDescription> tests;
	};
}
