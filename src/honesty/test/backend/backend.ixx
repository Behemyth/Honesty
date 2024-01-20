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
			friend class Registry;

			std::unordered_set<const Runner*> runners_;
			std::unordered_set<const Reporter*> reporters_;

			// TODO: Count with reflection using C++26
			const SuiteData* defaultSuites_[20]{};
		};

		static inline int size_ = 0;

	public:
		static Instance& GetInstance()
		{
			static Instance instance;
			return instance;
		}

		static void Add(const SuiteData* data)
		{
			Instance& instance = GetInstance();

			GetInstance().defaultSuites_[size_++] = data;
		}

		static void Add(SuiteData data, const Runner& runner)
		{
			GetInstance().runners_.insert(&runner);
		}

		static void AddReporter(const Reporter&)
		{
			GetInstance();
		}

		static void AddRunner(const Runner&)
		{
			GetInstance();
		}

		// static std::vector<SuiteData> ExtractDefaultData()
		//{
		//	return std::move(GetInstance().defaultSuites_);
		// }

		Registry()							 = default;
		Registry(const Registry&)			 = delete;
		Registry(Registry&&)				 = delete;
		Registry& operator=(const Registry&) = delete;
		Registry& operator=(Registry&&)		 = delete;
	};
}
