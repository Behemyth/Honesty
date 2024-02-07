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

			static constexpr int RUNNER_COUNT = 1;
			static constexpr int REPORTER_COUNT = 1;
			static constexpr int SUITE_COUNT = 20;

			void AddSuite(const SuiteData* const data)
			{
				suites_[suiteSize_++] = data;
			}

			void AddReporter(const Reporter* const reporter)
			{
				reporters_[reporterSize_++] = reporter;
			}

			void AddRunner(const Runner* const runner)
			{
				runners_[runnerSize_++] = runner;
			}

			std::span<const SuiteData* const> GetSuites() const
			{
				return std::span(suites_.data(), suiteSize_);
			}

			std::span<const Reporter* const> GetReporters() const
			{
				return std::span(reporters_.data(), reporterSize_);
			}

			std::span<const Runner* const> GetRunners() const
			{
				return std::span(runners_.data(), runnerSize_);
			}

		private:
			// TODO: Count with reflection using C++26

			std::array<const Runner*, RUNNER_COUNT> runners_{};
			std::array<const Reporter*, REPORTER_COUNT> reporters_{};
			std::array<const SuiteData*, SUITE_COUNT> suites_ {};

			static inline int suiteSize_ = 0;
			static inline int runnerSize_ = 0;
			static inline int reporterSize_ = 0;
		};

	public:
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

		static void AddReporter(const Reporter&)
		{
			GetInstance();
		}

		static void AddRunner(const Runner&)
		{
			GetInstance();
		}

		static std::span<const SuiteData* const> GetDefaultSuites()
		{
			return GetInstance().GetSuites();
		}

		static std::span<const Reporter* const> GetReporters()
		{
			return GetInstance().GetReporters();
		}

		static std::span<const Runner* const> GetRunners()
		{
			return GetInstance().GetRunners();
		}

		Registry()							 = default;
		Registry(const Registry&)			 = delete;
		Registry(Registry&&)				 = delete;
		Registry& operator=(const Registry&) = delete;
		Registry& operator=(Registry&&)		 = delete;
	};
}
