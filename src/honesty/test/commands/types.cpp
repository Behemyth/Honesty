module synodic.honesty.test.commands:types;

import std;
import synodic.honesty.log;
import :runner;
import :reporter;

namespace synodic::honesty::test::command
{
	struct Configuration
	{
		Configuration(const std::string_view applicationName, const log::Logger& logger) :
			applicationName(applicationName),
			logger(logger)
		{
		}

		std::string_view applicationName;

		std::reference_wrapper<const log::Logger> logger;
	};

	struct ParseResult
	{
		ParseResult(std::string runnerOverride, std::vector<std::string> reporterOverride) :
			runnerOverride(std::move(runnerOverride)),
			reporterOverrides(std::move(reporterOverride))
		{
		}

		std::string runnerOverride;
		std::vector<std::string> reporterOverrides;
	};

	struct ProcessConfiguration
	{
		ProcessConfiguration(Runner& runner, const std::span<std::unique_ptr<Reporter>> reporter) :
			runner(runner),
			reporters(reporter)
		{
		}

		std::reference_wrapper<Runner> runner;
		std::span<std::unique_ptr<Reporter>> reporters;
	};

	class Command
	{
	public:
		constexpr Command() = default;
		virtual ~Command() = default;

		Command(const Command& other) = delete;
		Command(Command&& other) noexcept = delete;
		auto operator=(const Command& other) -> Command& = delete;
		auto operator=(Command&& other) noexcept -> Command& = delete;

		virtual auto Parse(std::span<std::string_view> arguments) -> ParseResult = 0;
		virtual void Process(ProcessConfiguration& configuration) = 0;

	private:
	};

	template<typename T>
	concept command = requires(const Configuration& config)
	{
		// Constructable from a Configuration
		T(config);

		// The command must have a name, and it must be a string_view
		{
			T::NAME
		} -> std::same_as<std::string_view>;

		// The name must be constant
		{
			std::bool_constant<T::NAME>()
		} -> std::same_as<std::true_type>;

		// Must be derived from the Command class
		std::derived_from<T, Command>;
	};
}
