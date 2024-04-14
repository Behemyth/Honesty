export module synodic.honesty.test.runner.local;

import std;
import synodic.honesty.test;

namespace synodic::honesty::test
{
	export class Local final : public Runner
	{
	public:
		consteval explicit Local(std::string_view name);

		void Run(Broadcast& broadcaster) override;
		void Submit(const SuiteData* data) override;
		void Submit(std::span<const SuiteData* const> data) override;

	private:
		std::array<const SuiteData*, 30> suitesData_;
		std::span<const SuiteData*> suites_;
	};

	consteval Local::Local(std::string_view name) :
		Runner(name),
		suitesData_(),
		suites_(suitesData_.data(), 0)
	{
	}
}

module :private;

namespace synodic::honesty::test
{
	void Local::Run(Broadcast& broadcaster)
	{
		for (const SuiteData* const suite: suites_)
		{
			event::SuiteBegin event(suite->Name());

			broadcaster.signal(event);

			auto generator = suite->Generator();
			for (const TestBase& test: generator())
			{
				event::TestBegin beginEvent(test.Name());

				broadcaster.signal(beginEvent);

				test.Run();
			}
		}
	}

	void Local::Submit(const SuiteData* data)
	{
		suitesData_[suites_.size()] = std::move(data);

		suites_ = {suites_.data(), suites_.size() + 1};
	}

	void Local::Submit(std::span<const SuiteData* const> data)
	{
		for (const SuiteData* const suite: data)
		{
			suitesData_[suites_.size()] = std::move(suite);

			suites_ = {suites_.data(), suites_.size() + 1};
		}

		// suites_.insert(suites_.end(), std::make_move_iterator(data.begin()), std::make_move_iterator(data.end()));
	}
}
