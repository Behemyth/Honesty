export module synodic.honesty.test.backend;

import std;

import generator;

export namespace synodic::honesty
{
	class TestBase
	{
	public:
		virtual ~TestBase()		 = default;
		virtual void Run() const = 0
		{
			
		}

	private:
	};

	using Generator = std::generator<TestBase>;

	struct suite_data
	{
		suite_data(std::string_view name, Generator (*generator)()) noexcept;

		std::string_view name_;
		Generator (*generator_)();
	};

	suite_data::suite_data(std::string_view name, Generator (*generator)()) noexcept :
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
