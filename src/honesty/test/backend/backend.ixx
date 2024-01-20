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
			void AddSuite(const SuiteData& data)
			{
				defaultSuites_[size_++] = &data;
			}

			void AddRunner(const Runner& runner)
			{
				runners_.insert(&runner);
			}

			std::span<const SuiteData* const> GetDefaultSuites() const
			{
				return std::span(defaultSuites_.data(), size_);
			}

		private:
			std::unordered_set<const Runner*> runners_;
			std::unordered_set<const Reporter*> reporters_;

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

			GetInstance().AddSuite(data);
		}

		static void Add(const SuiteData& data, const Runner& runner)
		{
			auto& instance = GetInstance();

			instance.AddRunner(runner);
			instance.AddSuite(data);
		}

		static void AddReporter(const Reporter&)
		{
			GetInstance();
		}

		static void AddRunner(const Runner&)
		{
			GetInstance();
		}

		std::span<const SuiteData* const> GetDefaultSuites() const
		{
			return GetInstance().GetDefaultSuites();
		}

		Registry()							 = default;
		Registry(const Registry&)			 = delete;
		Registry(Registry&&)				 = delete;
		Registry& operator=(const Registry&) = delete;
		Registry& operator=(Registry&&)		 = delete;
	};
}
