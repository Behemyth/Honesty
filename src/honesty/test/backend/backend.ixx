export module synodic.honesty.test.backend;

import std;
export import :generator;
export import :test;
import function_ref;

export namespace synodic::honesty
{

	using TestGenerator = generator<TestBase>;

	struct suite_data
	{
		suite_data(std::string_view name, std::function_ref<TestGenerator()> generator) noexcept;

		std::string_view name_;
		std::function_ref<TestGenerator()> generator_;
	};

	suite_data::suite_data(std::string_view name, std::function_ref<TestGenerator()> generator) noexcept :
		name_(name),
		generator_(generator)
	{
	}

	class Registry
	{
		template<is_runner Runner>
		consteval Registry();

		class Instance()
		{
		public:

		private:
			std::set<std::function_ref<void()>> runners_;
		}

	public:
		static void Add(suite_data data)
		{
			Instance().data_.push_back(std::move(data));
		}

		static std::span<suite_data> DefaultedData()
		{
			return Instance().data_;
		}

	private:
		Registry() = default;

		static Registry& Instance(=)
		{
			if (registry_)
			{
				static Registry instance();
				registry_ = &instance;
				return instance;
			}

			return *registry_;
		}

		static Registry* registry_ = nullptr;
	};

	template<is_runner Runner>
	consteval Registry::Registry()
	{
	}
}
