export module synodic.honesty.test:interface;

import std;
import :registry;
import :reporter;
import :runner;
import :test;
import :logger;

namespace synodic::honesty::test
{
	export enum class TestResultCode : std::uint8_t
	{
		FAILURE
	};

	export struct ExecuteParameters
	{
		ExecuteParameters() = default;
	};

	export struct ListParameters
	{
		ListParameters() = default;
	};

	export struct ExecuteResult
	{
		ExecuteResult() = default;
	};

	export struct ListResult
	{
		ListResult() = default;
	};

	/**
	 * @brief The interface to interacting with the test framework. Multiple interfaces are allowed to exist for the
	 *	same instance
	 */
	export class Interface
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
}
