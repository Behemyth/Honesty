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

	class suite_data
	{
	public:
		consteval suite_data(std::string_view name, std::generator<TestBase> (*generator)()) noexcept;

	private:
		std::string_view name_;
		std::generator<TestBase> (*generator_)();
	};

	// std::span<suite_data> Suites();

	/**		
	 * \brief Counts the suites at compile time via type reflection. Using the resulting value needs to happen after this function
	 * \param name 
	 */
	consteval void RegisterSuite(std::string_view name, std::generator<TestBase> (*generator)())
	{
		suite_data::next<__COUNTER__>();
	}

	consteval void AddSuite(std::string_view name, std::generator<TestBase> (*generator)())
	{
		constexpr int size = counter.current<__COUNTER__>();

		//static std::array<suite_data, size> suites;


		// GetSuiteStorage().emplace_back(name, std::move(generator));
	}

	consteval suite_data::suite_data(std::string_view name, std::generator<TestBase> (*generator)()) noexcept :
		name_(name),
		generator_(std::move(generator))
	{
	}
}
