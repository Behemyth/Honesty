
export module synodic.honesty.test:runner;

import :types;
import :reporter;
import std;
import inplace_vector;
import :assert;

namespace synodic::honesty::test
{




	/**
	 * @brief The type of runner can be selected by the user when invoking tests, for example, via the command line
	 * interface. Additionally, the user can specify a specific runner for a specific test suite. Test suites must be
	 * registered with runtime registration to avoid having every translation unit recompile. With
	 * these two constraints its clear that the Runner interface must be runtime polymorphic.
	 *	Handles the interface to the test std::execution context.
	 */
	export class Runner
	{
	public:
		/**
		 * @brief
		 */
		explicit constexpr Runner(log::Logger logger) :
			logger_(std::move(logger))
		{
		}

		virtual ~Runner() = default;

		/**
		 * @brief
		 * @param requirements Test specifications
		 * @param function A function to execute on a scheduler
		 */
		virtual void Run(const Requirements& requirements, const std::function_ref<void(const Requirements&)> function) = 0;

		/**
		 * @brief Get the logger associated with this reporter
		 * @return The logger given at construction, or the root logger if one wasn't specified
		 */
		const log::Logger& Logger() const
		{
			// We don't use value_or here because the const reference is not convertible to a value
			if (logger_)
			{
				return logger_.value();
			}

			return log::RootLogger();
		}

	private:
		std::optional<log::Logger> logger_;
	};

	template<typename T>
	concept runner = requires {
		T::Name();
		{
			std::bool_constant<(T::Name(), true)>()
		} -> std::same_as<std::true_type>;
		std::derived_from<Runner, T>;
	};

	class RunnerRegistry
	{
	public:
		RunnerRegistry()
		{
			registrars_.push_back(this);
		}

		virtual ~RunnerRegistry() = default;

		virtual std::string_view Name() const							 = 0;
		virtual std::unique_ptr<Runner> Create(log::Logger logger) const = 0;

		static std::span<RunnerRegistry*> Registrars()
		{
			return registrars_;
		}

	private:
		constinit static std::inplace_vector<RunnerRegistry*, 2> registrars_;
	};

	constinit std::inplace_vector<RunnerRegistry*, 2> RunnerRegistry::registrars_;

	export template<runner T>
	class RunnerRegistrar final : RunnerRegistry
	{
	public:
		RunnerRegistrar()
		{
		}

		std::unique_ptr<Runner> Create(log::Logger logger) const override
		{
			return std::make_unique<T>(std::move(logger));
		}

		std::string_view Name() const override
		{
			return T::Name();
		}
	};
}
