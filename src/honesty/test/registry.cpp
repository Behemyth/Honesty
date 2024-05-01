module synodic.honesty.test:registry;

import :runner;
import :reporter;
import :suite;

import std;
import function_ref;

namespace synodic::honesty::test
{
	class Registry
	{
		static constexpr int RUNNER_COUNT = 2;
		static constexpr int REPORTER_COUNT = 2;
		static constexpr int SUITE_COUNT = 20;

	public:
		constexpr Registry() = default;
		~Registry() = default;

		void AddSuite(SuiteView data)
		{
			suites_[suiteSize_++] = std::move(data);
		}

		void AddReporter(Reporter& reporter)
		{
			reporters_[reporterSize_++] = &reporter;
		}

		void AddRunner(Runner& runner)
		{
			runners_[runnerSize_++] = &runner;
		}

		std::span<SuiteView> GetSuites()
		{
			return std::span(suites_.data(), suiteSize_);
		}

		std::span<Reporter*> GetReporters()
		{
			return std::span(reporters_.data(), reporterSize_);
		}

		std::span<Runner*> GetRunners()
		{
			return std::span(runners_.data(), runnerSize_);
		}

		Registry(const Registry&) = delete;
		Registry(Registry&&) = delete;
		Registry& operator=(const Registry&) = delete;
		Registry& operator=(Registry&&) = delete;

	private:
		// Compile-time storage of run-time objects
		// TODO: Count with reflection using C++26

		std::array<Runner*, RUNNER_COUNT> runners_{};
		std::array<Reporter*, REPORTER_COUNT> reporters_{};
		std::array<SuiteView, SUITE_COUNT> suites_{};

		int suiteSize_ = 0;
		int runnerSize_ = 0;
		int reporterSize_ = 0;
	};
}

namespace
{
	constinit synodic::honesty::test::Registry REGISTRY;
}

synodic::honesty::test::Registry& GetRegistry()
{
	return REGISTRY;
}
