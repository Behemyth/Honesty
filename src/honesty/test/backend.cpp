module synodic.honesty.test:backend;

import :backend;

namespace synodic::honesty
{
	class suite_data
	{
	public:
		suite_data(std::string_view name, std::move_only_function<Generator()> generator);

	private:
		std::string_view name_;
		std::move_only_function<Generator()> generator_;
	};

	suite_data::suite_data(std::string_view name, std::move_only_function<Generator()> generator) :
		name_(name),
		generator_(std::move(generator))
	{
	}
}

namespace
{
	std::vector<synodic::honesty::suite_data> suites;
}

namespace synodic::honesty
{
}
