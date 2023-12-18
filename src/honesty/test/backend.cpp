module synodic.honesty.test.backend;

namespace
{
	std::vector<synodic::honesty::suite_data> suites;
}

namespace synodic::honesty
{
	suite_data::suite_data(std::string_view name, std::move_only_function<std::generator<TestBase>()> generator) :
		name_(name),
		generator_(std::move(generator))
	{
	}

	std::span<suite_data> Suites()
	{
		return suites;
	}

	void AddSuite(std::string_view name, std::move_only_function<std::generator<TestBase>()> generator)
	{
		suites.emplace_back(name, std::move(generator));
	}
}
