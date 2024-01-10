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
			Instance()//:
				//defaultSuites_()
			{
			}

		private:
			friend class Registry;

			std::unordered_set<const Runner*> runners_;
			std::unordered_set<const Reporter*> reporters_;

			// TODO: Count with reflection using C++26
			//suite_data defaultSuites_[20];
		};

	public:
		static Instance& GetInstance()
		{
			static Instance instance;
			return instance;
		}

		static void Add(suite_data&& data)
		{
			Instance& instance = GetInstance();

			constexpr int index = instance.next<__COUNTER__>();
			//GetInstance().defaultSuites_[index] = std::move(data);
		}

		static void Add(suite_data data, const Runner& runner)
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

		//static std::vector<suite_data> ExtractDefaultData()
		//{
		//	return std::move(GetInstance().defaultSuites_);
		//}

		Registry()							 = default;
		Registry(const Registry&)			 = delete;
		Registry(Registry&&)				 = delete;
		Registry& operator=(const Registry&) = delete;
		Registry& operator=(Registry&&)		 = delete;

	};
}
