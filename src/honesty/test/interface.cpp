module synodic.honesty.test:interface;

import std;
import :registry;
import :reporter;
import :runner;
import :test;
import :logger;

namespace synodic::honesty::test
{
	enum class TestResultCode : std::uint8_t
	{
		FAILURE
	};

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
		enum class Command
		{
			EXECUTE,
			LIST
		};

		struct Configuration
		{
			Configuration() = default;
		};

		Interface();

		result::Execute Execute(const Configuration& parameters);

		result::List List(const Configuration& parameters);

	private:
		std::span<Reporter*> reporters_;
		std::span<Runner*> runners_;
	};

	Interface::Interface()
	{
		Registry& registry = GetRegistry();

		reporters_ = registry.GetReporters();
		runners_ = registry.GetRunners();
	}

	result::Execute Interface::Execute(const Configuration& parameters)
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

	result::List Interface::List(const Configuration& parameters)
	{
		return {};
	}
}
