export module synodic.honesty.test:api;

import synodic.honesty.log;
import std;
import :suite;
import :context;
import :reporter;
import :runner;
import :test;
import :types;

import :api.execute;
import :api.list;

namespace synodic::honesty::test
{
	export enum class TestResultCode : std::uint8_t
	{
		FAILURE
	};

	export struct HelpParameters
	{
		HelpParameters() = default;
	};

	export struct TestDescription
	{
		TestDescription() = default;

		std::string name;
	};

	export struct SuiteDescription
	{
		SuiteDescription() = default;

		std::string name;
		std::vector<TestDescription> tests;
	};

	/**
	 * @brief The api to interacting with the test framework. Multiple interfaces are allowed to exist for the
	 *	same instance.
	 */
	export class Interface
	{
	public:
		struct Configuration
		{
			explicit Configuration(std::string applicationName) :
				applicationName(std::move(applicationName))
			{
			}

			std::string applicationName;
		};

		explicit Interface(const Configuration& configuration) :
			applicationName_(configuration.applicationName)
		{
		}

		[[nodiscard]] ExecuteResult Execute(const ExecuteParameters& parameters)
		{
			return api::Execute(parameters);
		}

		[[nodiscard]] ListResult List(const ListParameters& parameters)
		{
			return api::List(parameters);
		}

	private:
		std::string applicationName_;
	};
}
