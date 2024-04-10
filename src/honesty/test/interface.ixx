
export module synodic.honesty.test:interface;

import std;
import :registry;
import :reporter;
import :runner;
import :test;

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

	/**
	 * @brief The interface to interacting with the test framework. Multiple interfaces are allowed to exist for the
	 *	same instance
	 */
	export class Interface
	{
	public:
		template<typename ReporterT, typename RunnerT>
		Interface(ReporterT& defaultReporter, RunnerT& defaultRunner);

		result::Execute Execute(const parameter::Execute& parameters);

		result::List List(const parameter::List& parameters);

	private:
		std::span<Reporter*> reporters_;
		std::span<Runner*> runners_;
	};

	template<typename ReporterT, typename RunnerT>
	Interface::Interface(ReporterT& defaultReporter, RunnerT& defaultRunner)
	{
		// TODO: Change once multiple reporters/runners are supported correctly
		defaultRunner.Submit(GetRegistry().GetSuites());

		Registry& registry = GetRegistry();

		registry.AddReporter(defaultReporter);
		registry.AddRunner(defaultRunner);

		reporters_ = registry.GetReporters();
		runners_   = registry.GetRunners();
	}
}
