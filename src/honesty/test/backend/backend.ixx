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
			void AddSuite(const SuiteData* const data)
			{
				defaultSuites_[size_++] = data;
			}

			//void AddRunner(const Runner& runner)
			//{
			//	runners_.insert(&runner);
			//}

			std::span<const SuiteData* const> GetDefaultSuites() const
			{
				return std::span(defaultSuites_.data(), size_);
			}

			std::span<const Reporter* const> GetReporters() const
			{
				return reporters_;
			}

			std::span<const Runner* const> GetRunners() const
			{
				return runners_;
			}

		private:
			std::vector<const Runner*> runners_;
			std::vector<const Reporter*> reporters_;

			// TODO: Count with reflection using C++26
			std::array<const SuiteData*, 20> defaultSuites_ {};

			static inline int size_ = 0;
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
			return GetInstance().GetDefaultSuites();
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
