export module synodic.honesty.test.backend;

import std;

import generator;

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
		suite_data(std::string_view name, std::move_only_function<std::generator<TestBase>()> generator) noexcept;

	private:
		std::string_view name_;
		std::move_only_function<std::generator<TestBase>()> generator_;
	};

	std::span<suite_data> Suites();
	void AddSuite(std::string_view name, std::move_only_function<std::generator<TestBase>()> generator);

}
