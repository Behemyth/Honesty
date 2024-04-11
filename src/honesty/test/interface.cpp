module synodic.honesty.test:interface;

import std;
import :registry;
import :reporter;
import :runner;
import :test;

namespace synodic::honesty::test
{
	enum class TestResultCode : std::uint8_t
	{
		FAILURE
	};

	// Parameters inputs for the Interface
	namespace parameter
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

	namespace result
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
	class Interface
	{
	public:
		Interface(std::span<Reporter*> reporters, std::span<Runner*> runners);

		result::Execute Execute(const parameter::Execute& parameters);

		result::List List(const parameter::List& parameters);

	private:
		std::span<Reporter*> reporters_;
		std::span<Runner*> runners_;
	};

	Interface::Interface(std::span<Reporter*> reporters, std::span<Runner*> runners)
	{
		test::logger::Console logger;
		test::reporter::Console reporter(logger);
		test::runner::Local runner;

		// TODO: Change once multiple reporters/runners are supported correctly
		defaultRunner.Submit(GetRegistry().GetSuites());

		Registry& registry = GetRegistry();

		registry.AddReporter(defaultReporter);
		registry.AddRunner(defaultRunner);

		reporters_ = registry.GetReporters();
		runners_ = registry.GetRunners();
	}

	result::Execute Interface::Execute(const parameter::Execute& parameters)
	{
		RunnerContext& context = GetContext();

		// Set the context
		context = RunnerContext(reporters_);

		for (Runner* runner: runners_)
		{
			runner->Run(context.broadcaster);
		}

		return {};
	}

	result::List Interface::List(const parameter::List& parameters)
	{
		return {};
	}
}
