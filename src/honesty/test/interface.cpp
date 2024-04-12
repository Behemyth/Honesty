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

	struct ExecuteParameters
	{
		ExecuteParameters() = default;
	};

	struct ListParameters
	{
		ListParameters() = default;
	};

	struct ExecuteResult
	{
		ExecuteResult() = default;
	};

	struct ListResult
	{
		ListResult() = default;
	};

	/**
	 * @brief The interface to interacting with the test framework. Multiple interfaces are allowed to exist for the
	 *	same instance
	 */
	class Interface
	{
	public:
		struct Configuration
		{
			Configuration() = default;
		};

		Interface();

		ExecuteResult Execute(const ExecuteParameters& parameters);

		ListResult List(const ListParameters& parameters);

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

	ExecuteResult Interface::Execute(const ExecuteParameters& parameters)
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

	ListResult Interface::List(const ListParameters& parameters)
	{
		return {};
	}
}
