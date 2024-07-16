export module synodic.honesty.test.api:list;

import std;

import synodic.honesty.log;
import synodic.honesty.test;

import :types;
import :execute;

class ListReporter final : public synodic::honesty::test::CumulativeAdapter
{
public:
	explicit(false) ListReporter(const synodic::honesty::log::Logger& logger) :
		CumulativeAdapter(logger)
	{
	}

	~ListReporter() override = default;

	static consteval std::string_view Name()
	{
		return "list";
	}

	void Finalize(CumulativeData data) override
	{
		data_ = std::move(data);
	}

	const CumulativeData& Data() const
	{
		return data_;
	}

private:
	CumulativeData data_;
};

namespace synodic::honesty::test::api
{
	export struct ListParameters
	{
		explicit ListParameters(const std::string_view applicationName, Runner& runner, const log::Logger& logger) :
			applicationName(applicationName),
			runner(runner),
			logger(logger)
		{
		}

		std::string_view applicationName;

		std::reference_wrapper<Runner> runner;
		std::span<std::unique_ptr<Reporter>> reporters;

		std::reference_wrapper<const log::Logger> logger;
	};

	export struct ListResult
	{
		ListResult() = default;

		std::vector<SuiteDescription> suites;
	};

	export auto List(const ListParameters& parameters) -> ListResult
	{
		// Register the list reporters and immediately grab it from the registration list
		{
			static ReporterRegistrar<ListReporter> listReporterRegistrar;
		}

		// TODO: Filter with list command
		const ExecuteParameters executeParameters(
			parameters.applicationName,
			"",
			parameters.runner,
			parameters.reporters,
			true,
			parameters.logger);

		ExecuteResult executeResult = Execute(executeParameters);

		ListResult result;

		// Grab the list reporter and extract the data
		std::unique_ptr<Reporter>& abstractReporter	  = parameters.reporters.front();
		ListReporter& listReporter					  = dynamic_cast<ListReporter&>(*abstractReporter);

		const CumulativeAdapter::CumulativeData& data = listReporter.Data();

		for (const auto& [name, tests]: data.suites)
		{
			auto& resultSuite = result.suites.emplace_back();
			resultSuite.name  = name;

			for (const auto& [name]: tests)
			{
				TestDescription description;
				description.name = name;

				resultSuite.tests.push_back(description);
			}
		}

		return result;
	}
}
