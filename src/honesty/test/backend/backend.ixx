export module synodic.honesty.test.backend;

export import :generator;
export import :test;
export import :runner;
export import :reporter;
export import :suite;

import std;
import function_ref;

export namespace synodic::honesty
{
	using TestGenerator = generator<TestBase>;

	class Registry
	{
		class Instance
		{
		public:

		private:
			friend class Registry;

			std::unordered_set<const Runner*> runners_;
			std::vector<suite_data> defaultSuites_;
		};

	public:
		static Instance& GetInstance()
		{
			static Instance instance;
			return instance;
		}

		static void Add(suite_data&& data)
		{
			GetInstance().defaultSuites_.push_back(std::move(data));
		}

		static void Add(suite_data data, const Runner& runner)
		{
			GetInstance().runners_.insert(&runner);
		}

		std::vector<suite_data> ExtractDefaultData() const
		{
			return std::move(GetInstance().defaultSuites_);
		}

		Registry()							 = default;
		Registry(const Registry&)			 = delete;
		Registry(Registry&&)				 = delete;
		Registry& operator=(const Registry&) = delete;
		Registry& operator=(Registry&&)		 = delete;
	};
}
