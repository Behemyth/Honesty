export module synodic.honesty.test.backend:reporter_registry;

import std;

import inplace_vector;

import synodic.honesty.log;
import :reporter;

namespace honesty::test
{
	constexpr std::size_t DEFAULT_REPORTER_CAPACITY = 4;

	export class ReporterRegistry
	{
	public:
		ReporterRegistry()
		{
			registrars_.push_back(this);
		}

		virtual ~ReporterRegistry() = default;

		virtual std::string_view Name() const									  = 0;
		virtual std::unique_ptr<Reporter> Create(const log::Logger& logger) const = 0;

		static void Register(ReporterRegistry* registry)
		{
			registrars_.push_back(registry);
		}

		static std::span<ReporterRegistry*> Registrars()
		{
			return registrars_;
		}

	private:
		constinit static std::inplace_vector<ReporterRegistry*, DEFAULT_REPORTER_CAPACITY> registrars_;
	};

	constinit std::inplace_vector<ReporterRegistry*, DEFAULT_REPORTER_CAPACITY> ReporterRegistry::registrars_;
}
