export module synodic.honesty.test.backend;

import std;

import generator;
import counter;

export namespace synodic::honesty
{
	class TestBase
	{
	public:
		virtual ~TestBase() = default;
		virtual void Run()	= 0;

	private:
	};

	template<int T>
	class suite_data
	{
	public:

		static consteval void Initialize(std::string_view name, std::generator<TestBase> (*generator)());
		static consteval const suite_data& Get();

	private:
		consteval suite_data(std::string_view name, std::generator<TestBase> (*generator)()) noexcept;

		static consteval suite_data& SetData(std::string_view name, std::generator<TestBase> (*generator)());

		std::string_view name_;
		std::generator<TestBase> (*generator_)();
	};

	template<int T>
	consteval suite_data<T>::suite_data(std::string_view name, std::generator<TestBase> (*generator)()) noexcept :
		name_(name),
		generator_(std::move(generator))
	{
	}

	template<int T>
	consteval void suite_data<T>::Initialize(std::string_view name, std::generator<TestBase> (*generator)())
	{
		SetData(name, generator);
	}

	template<int T>
	consteval const suite_data<T>& suite_data<T>::Get()
	{
		return SetData(suite_data("Uninitialized Suite", []{}));
	}

	template<int T>
	consteval suite_data<T>& suite_data<T>::SetData(std::string_view name, std::generator<TestBase> (*generator)())
	{
		static constexpr suite_data data(name, generator);
		return data;
	}


	// std::span<suite_data> Suites();

	/**
	 * \brief Counts the suites at compile time via type reflection. Using the resulting value needs to happen after
	 * this function \param name
	 */
	consteval void AddSuite(std::string_view name, std::generator<TestBase> (*generator)())
	{
		// constexpr int size = counter.current<__COUNTER__>();

		// static std::array<suite_data, size> suites;

		// GetSuiteStorage().emplace_back(name, std::move(generator));
	}

}
