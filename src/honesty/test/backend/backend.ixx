export module synodic.honesty.test.backend;

import std;
export import :generator;
export import :test;
import function_ref;

export namespace synodic::honesty
{

	using TestGenerator = std::generator<TestBase>;

	struct suite_data
	{
		suite_data(std::string_view name,std::function_ref<TestGenerator()> generator) noexcept;

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
	public:
		static void Add(suite_data data)
		{
			Instance().data_.push_back(std::move(data));
		}

		static std::span<suite_data> GenerateSuites()
		{
			return Instance().data_;
		}

	private:
		Registry() = default;

		static Registry& Instance()
		{
			static Registry instance;
			return instance;
		}

		std::vector<suite_data> data_;
	};

}
