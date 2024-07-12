
export module synodic.honesty.test:runner;

import synodic.honesty.test.backend;

import :reporter;
import std;
import inplace_vector;



namespace synodic::honesty::test
{
	export using class synodic::honesty::test::Runner;

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

		virtual std::string_view Name() const									= 0;
		virtual std::unique_ptr<Runner> Create(const log::Logger& logger) const = 0;

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

		std::unique_ptr<Runner> Create(const log::Logger& logger) const override
		{
			return std::make_unique<T>(logger);
		}

		std::string_view Name() const override
		{
			return T::Name();
		}
	};
}
