export module synodic.honesty.test.backend;

export import :generator;
export import :test;
export import :runner;
export import :logger;
export import :reporter;
export import :suite;

import std;
import function_ref;
import counter;

export namespace synodic::honesty
{
	using TestGenerator = generator<TestBase>;

	class Registry
	{
		class Instance : std::counter<Instance>
		{
		public:
			static constexpr int RUNNER_COUNT	= 1;
			static constexpr int REPORTER_COUNT = 1;
			static constexpr int SUITE_COUNT	= 20;

			void AddSuite(const SuiteData* const data)
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

			std::span<const SuiteData* const> GetSuites() const
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
			// TODO: Count with reflection using C++26

			std::array<Runner*, RUNNER_COUNT> runners_ {};
			std::array<Reporter*, REPORTER_COUNT> reporters_ {};
			std::array<const SuiteData*, SUITE_COUNT> suites_ {};

			static inline int suiteSize_	= 0;
			static inline int runnerSize_	= 0;
			static inline int reporterSize_ = 0;
		};

	public:
		template<typename ReporterT, typename RunnerT>
		Registry(ReporterT& reporter, RunnerT& runner);

		static Instance& GetInstance()
		{
			static Instance instance;
			return instance;
		}

		static void Add(const SuiteData& data)
		{
			Instance& instance = GetInstance();

			GetInstance().AddSuite(&data);
		}

		static void Add(const SuiteData& data, const Runner& runner)
		{
			auto& instance = GetInstance();
		}

		static void AddReporter(Reporter& reporter)
		{
			GetInstance().AddReporter(reporter);
		}

		static void AddRunner(Runner& runner)
		{
			GetInstance().AddRunner(runner);
		}

		std::span<Reporter*> GetReporters()
		{
			return GetInstance().GetReporters();
		}

		std::span<Runner*> GetRunners()
		{
			return GetInstance().GetRunners();
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
		runner.Submit(GetInstance().GetSuites());

		AddRunner(runner);
	}
}
