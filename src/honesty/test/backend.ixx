export module synodic.honesty.test:backend;

import :runner;
import :reporter;
import :types;

import std;
import function_ref;

namespace synodic::honesty::test
{
	class Registry
	{
		class Instance
		{
		public:
			consteval Instance() = default;

			static constexpr int RUNNER_COUNT	= 1;
			static constexpr int REPORTER_COUNT = 1;
			static constexpr int SUITE_COUNT	= 20;

			void AddSuite(SuiteData* data)
			{
				suites_[suiteSize_++] = data;
			}

			void AddReporter(Reporter& reporter)
			{
				reporters_[reporterSize_++] = &reporter;
			}

			void AddRunner(Runner& runner)
			{
				runners_[runnerSize_++] = &runner;
			}

			std::span<SuiteData*> GetSuites()
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

		private:
			// Compile-time storage of run-time objects
			// TODO: Count with reflection using C++26

			std::array<Runner*, RUNNER_COUNT> runners_ {};
			std::array<Reporter*, REPORTER_COUNT> reporters_ {};
			std::array<SuiteData*, SUITE_COUNT> suites_ {};

			int suiteSize_	  = 0;
			int runnerSize_	  = 0;
			int reporterSize_ = 0;
		};

		// Our singleton instance
		constinit static inline Instance instance;

		// The fallback context for tests
		constinit static inline RunnerContext defaultContext;

		// Each thread has its own context, such that tests can reference global functions without an object
		constinit static inline thread_local RunnerContext& context = defaultContext;


	public:
		template<typename ReporterT, typename RunnerT>
		Registry(ReporterT& reporter, RunnerT& runner);

		static void Add(SuiteData& data)
		{
			instance.AddSuite(&data);
		}

		static void Add(SuiteData& data, const Runner& runner)
		{
		}

		static void AddReporter(Reporter& reporter)
		{
			instance.AddReporter(reporter);
		}

		static void AddRunner(Runner& runner)
		{
			instance.AddRunner(runner);
		}

		static std::span<Reporter*> GetReporters()
		{
			return instance.GetReporters();
		}

		static std::span<Runner*> GetRunners()
		{
			return instance.GetRunners();
		}

		static RunnerContext& Context()
		{
			return context;
		}

		Registry(const Registry&)			 = delete;
		Registry(Registry&&)				 = delete;
		Registry& operator=(const Registry&) = delete;
		Registry& operator=(Registry&&)		 = delete;
	};

	template<typename ReporterT, typename RunnerT>
	Registry::Registry(ReporterT& reporter, RunnerT& runner)
	{
		AddReporter(reporter);

		// Add the default collected suites to the default runner if it's provided
		runner.Submit(instance.GetSuites());

		AddRunner(runner);
	}
}
