
export module synodic.honesty.test:interface;

import std;
import :backend;
import :reporter;
import :runner;

namespace synodic::honesty
{
}

namespace synodic::honesty::test
{
	export enum class TestResultCode : std::uint8_t
	{
		FAILURE
	};

	// Parameters inputs for the API
	export namespace parameters
	{
		struct Execute
		{
			Execute() = default;
		};

		struct List
		{
			List() = default;
		};
	}

	export class API
	{
	public:
		template<typename ReporterT, typename RunnerT>
		API(ReporterT& defaultReporter, RunnerT& defaultRunner);

		void Execute(const parameters::Execute& parameters);

		void List(const parameters::List& parameters);

	private:
		Registry registry;
		std::span<Reporter*> reporters_;
		std::span<Runner*> runners_;
	};

	template<typename ReporterT, typename RunnerT>
	API::API(ReporterT& defaultReporter, RunnerT& defaultRunner) :
		registry(defaultReporter, defaultRunner),
		reporters_(registry.GetReporters()),
		runners_(registry.GetRunners())
	{
	}
}
