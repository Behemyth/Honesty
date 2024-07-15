
export module synodic.honesty.test.backend:runner;

import std;

import inplace_vector;

import synodic.honesty.log;
import synodic.honesty.test.types;

namespace synodic::honesty::test
{

	export class RunnerRegistry
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
}
