import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;
using namespace synodic::honesty::test::literals;

namespace
{
	constexpr Suite SUITE(
		"suite",
		[](const Fixture& fixture) -> Generator
		{
			co_yield "name"_test = [&](const Requirements& requirements)
			{
				requirements.ExpectEquals(fixture.SuiteName(), "suite");

				requirements.ExpectThrow<std::runtime_error>(
					[]
					{
						// Empty should fail
						VerifySuiteName("");
					});

				requirements.ExpectThrow<std::runtime_error>(
					[]
					{
						// Uppercase letters should fail
						VerifySuiteName("ThisFails");
					});

				requirements.ExpectThrow<std::runtime_error>(
					[]
					{
						// Spaces should fail
						VerifySuiteName("this fails");
					});

				// Only alphanumeric characters should pass, excluding '.'
				requirements.ExpectNotThrow(
					[]
					{
						VerifySuiteName("this.passes");
					});

				requirements.ExpectThrow<std::runtime_error>(
					[]
					{
						VerifySuiteName("this!fails");
					});
			};
		});

	constexpr Suite TEST_SUITE(
		"duplicate",
		[](const Fixture& fixture) -> Generator
		{
			co_yield "name"_test = [&](const Requirements& requirements)
			{
				requirements.ExpectEquals(fixture.SuiteName(), "duplicate");

				// Suites are registered in-order. This suite should fail due to duplicated name
				requirements.ExpectThrow<std::runtime_error>(
					[]
					{
						VerifySuiteName("suite");
					});
			};
		});

	SuiteRegistrar _(SUITE, TEST_SUITE);
}
