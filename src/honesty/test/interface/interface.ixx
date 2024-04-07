
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

	// Parameters inputs for the Interface
	export namespace parameter
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

	export namespace result
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

	export class Interface
	{
	public:
		template<typename ReporterT, typename RunnerT>
		Interface(ReporterT& defaultReporter, RunnerT& defaultRunner);

		result::Execute Execute(const parameter::Execute& parameters);

		result::List List(const parameter::List& parameters);

	private:
		Registry registry;
		std::span<Reporter*> reporters_;
		std::span<Runner*> runners_;
	};

	template<typename ReporterT, typename RunnerT>
	Interface::Interface(ReporterT& defaultReporter, RunnerT& defaultRunner) :
		registry(defaultReporter, defaultRunner),
		reporters_(registry.GetReporters()),
		runners_(registry.GetRunners())
	{
	}
}
