export module synodic.honesty.test.api:list;

import std;

import synodic.honesty.log;
import synodic.honesty.test;
import synodic.honesty.test.backend;

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

		std::vector<std::unique_ptr<Reporter>> reporters;

		// TODO: Create a path for direct initialization
		{
			const std::span<ReporterRegistry*> reporterRegistrars = ReporterRegistry::Registrars();
			auto iterator										  = std::ranges::find_if(
				reporterRegistrars,
				[&](const ReporterRegistry* registry) -> bool
				{
					return registry->Name() == "list";
				});

			const ReporterRegistry* registry = *iterator;

			reporters.push_back(registry->Create(parameters.logger));
		}

		// TODO: Filter with list command
		const ExecuteParameters
			executeParameters(parameters.applicationName, "", parameters.runner, reporters, true, parameters.logger);

		ExecuteResult executeResult = Execute(executeParameters);

		ListResult result;

		// Grab the list reporter and extract the data
		std::unique_ptr<Reporter>& abstractReporter = reporters.front();
		ListReporter& listReporter					= dynamic_cast<ListReporter&>(*abstractReporter);

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
